// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2001-2003 International Computer Science Institute
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software")
// to deal in the Software without restriction, subject to the conditions
// listed in the XORP LICENSE file. These conditions include: you must
// preserve this copyright notice, and you cannot mention the copyright
// holders in advertising related to the Software without their permission.
// The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
// notice is a summary of the XORP LICENSE file; the license in that file is
// legally binding.

#ident "$XORP: xorp/mfea/mfea_vif.cc,v 1.2 2003/03/10 23:20:41 hodson Exp $"


//
// MFEA virtual interfaces implementation.
//


#include "mfea_module.h"
#include "mfea_private.hh"
#include "mfea_node.hh"
#include "mfea_unix_osdep.hh"
#include "mfea_vif.hh"


//
// Exported variables
//

//
// Local constants definitions
//

//
// Local structures/classes, typedefs and macros
//

//
// Local variables
//

//
// Local functions prototypes
//


/**
 * MfeaVif::MfeaVif:
 * @mfea_node: The MFEA node this interface belongs to.
 * @vif: The generic Vif interface that contains various information.
 * 
 * MFEA vif constructor.
 **/
MfeaVif::MfeaVif(MfeaNode& mfea_node, const Vif& vif)
    : ProtoUnit(mfea_node.family(), mfea_node.module_id()),
      Vif(vif),
      _mfea_node(mfea_node)
{
    _min_ttl_threshold = MULTICAST_MIN_TTL_THRESHOLD_DEFAULT;
    _max_rate_limit = 0;	// XXX: unlimited rate limit
}

/**
 * MfeaVif::MfeaVif:
 * @mfea_node: The MFEA node this interface belongs to.
 * @mfea_vif: The origin MfeaVif interface that contains the initialization
 * information.
 * 
 * MFEA vif copy constructor.
 **/
MfeaVif::MfeaVif(MfeaNode& mfea_node, const MfeaVif& mfea_vif)
    : ProtoUnit(mfea_node.family(), mfea_node.module_id()),
      Vif(mfea_vif),
      _mfea_node(mfea_node)
{
    // Copy the MfeaVif-specific information
    set_min_ttl_threshold(mfea_vif.min_ttl_threshold());
    set_max_rate_limit(mfea_vif.max_rate_limit());
}

/**
 * MfeaVif::~MfeaVif:
 * @void: 
 * 
 * MFEA vif destructor.
 * 
 **/
MfeaVif::~MfeaVif(void)
{
    stop();
}

/**
 * MfeaVif::start:
 * @void: 
 * 
 * Start MFEA a single virtual interface.
 * 
 * Return value: %XORP_OK on success, otherwise %XORP_ERROR.
 **/
int
MfeaVif::start(void)
{
    if (! is_underlying_vif_up())
	return (XORP_ERROR);
    
    if (ProtoUnit::start() < 0)
	return (XORP_ERROR);
    
    //
    // Install in the kernel only if the vif is of the appropriate type:
    // multicast-capable (loopback excluded), or PIM Register vif.
    //
    if ((is_multicast_capable() && (! is_loopback()))
	|| is_pim_register()) {
	if (mfea_node().add_multicast_vif(vif_index()) < 0)
	    return (XORP_ERROR);
	return (XORP_OK);
    }
    
    return (XORP_ERROR);
}

/**
 * MfeaVif::stop:
 * @void: 
 * 
 * Stop MFEA on a single virtual interface.
 * 
 * Return value: %XORP_OK on success, otherwise %XORP_ERROR.
 **/
int
MfeaVif::stop(void)
{
    int ret_value = XORP_OK;
    
    leave_all_multicast_groups();
    
    if (ProtoUnit::stop() < 0)
	return (XORP_ERROR);
    
    // Remove from the kernel only if the vif is of the appropriate type
    if (is_multicast_capable() && (! is_loopback()))
	ret_value = mfea_node().delete_multicast_vif(vif_index());
    
    return (ret_value);
}

/**
 * MfeaVif::start_protocol:
 * @module_instance_name: The module instance name of the protocol to start
 * on this vif.
 * @module_id: The #xorp_module_id of the protocol to start on this vif.
 * 
 * Start a protocol on a single virtual vif.
 * 
 * Return value: %XORP_OK on success, otherwise %XORP_ERROR.
 **/
int
MfeaVif::start_protocol(const string& module_instance_name,
			xorp_module_id module_id)
{
    if (! is_valid_module_id(module_id)) {
	XLOG_ERROR("Cannot start protocol instance %s on vif %s: "
		   "invalid module_id = %d",
		   module_instance_name.c_str(), name().c_str(), module_id);
	return (XORP_ERROR);
    }
    
    if (_proto_register.add_protocol(module_instance_name, module_id) < 0)
	return (XORP_ERROR);
    
    //
    // Start the protocol in the MfeaNode
    // (XXX: may be called more than once)
    if (mfea_node().start_protocol(module_id) < 0) {
	_proto_register.delete_protocol(module_instance_name, module_id);
	return (XORP_ERROR);
    }
    
    // TODO: should we implicitly start the vif? Maybe no, because is bad
    // semantics...?
#if 0
    if (! is_up())
	start();	// XXX: start the vif
#endif // 0
    
    return (XORP_OK);
}

/**
 * MfeaVif::stop_protocol:
 * @module_instance_name: The module instance name of the protocol to stop
 * on this vif.
 * @module_id: The #xorp_module_id of the protocol to stop on this vif.
 * 
 * Stop a protocol on this vif.
 * 
 * Return value: %XORP_OK on success, otherwise %XORP_ERROR.
 **/
int
MfeaVif::stop_protocol(const string& module_instance_name,
		       xorp_module_id module_id)
{
    if (! is_valid_module_id(module_id)) {
	XLOG_ERROR("Cannot stop protocol instance %s on vif %s: "
		   "invalid module_id = %d",
		   module_instance_name.c_str(), name().c_str(), module_id);
	return (XORP_ERROR);
    }
    
    leave_all_multicast_groups(module_instance_name, module_id);
    
    if (_proto_register.delete_protocol(module_instance_name, module_id) < 0)
	return (XORP_ERROR);
    
    // TODO: should we implicitly stop the vif? Maybe no, because is bad
    // semantics...?
#if 0
    //
    // Test if we should stop the vif
    //
    bool do_stop = true;
    for (size_t i = 0; i < XORP_MODULE_MAX; i++) {
	if (_proto_register.is_registered(static_cast<xorp_module_id>(i))) {
	    do_stop = false;		// Vif is still in use by a protocol
	    break;
	}
    }
    
    if (is_up() && do_stop)
	stop();		// Stop the vif
#endif // 0
    
    return (XORP_OK);
}

/**
 * MfeaVif::leave_all_multicast_groups:
 * 
 * Leave all previously joined multicast groups on this interface.
 * 
 * Return value: The number of groups that were successfully left,
 * or %XORP_ERROR if error.
 **/
int
MfeaVif::leave_all_multicast_groups()
{
    int ret_value = 0;
    
    // Get a copy of the joined multicast state
    list<pair<pair<string, xorp_module_id>, IPvX> > joined_state
	= _joined_groups.joined_state();
    list<pair<pair<string, xorp_module_id>, IPvX> >::iterator iter;
    
    // Remove all the joined state and leave the groups
    for (iter = joined_state.begin(); iter != joined_state.end(); ++iter) {
	pair<string, xorp_module_id>& proto_state = (*iter).first;
	string& tmp_module_instance_name = proto_state.first;
	xorp_module_id tmp_module_id = proto_state.second;
	IPvX& group = (*iter).second;
	
	if (mfea_node().leave_multicast_group(tmp_module_instance_name,
					      tmp_module_id,
					      vif_index(),
					      group) == XORP_OK)
	    ret_value++;
    }
    
    return (ret_value);
}

/**
 * MfeaVif::leave_all_multicast_groups:
 * @module_instance_name: The module instance name of the protocol that
 * leaves all groups.
 * @module_id: The module ID of the protocol that leaves all groups.
 * 
 * Leave all previously joined multicast groups on this interface
 * that were joined by a module with name @module_instance_name
 * and module ID @module_id.
 * 
 * Return value: The number of groups that were successfully left,
 * or %XORP_ERROR if error.
 **/
int
MfeaVif::leave_all_multicast_groups(const string& module_instance_name,
				    xorp_module_id module_id)
{
    int ret_value = 0;
    
    // Get a copy of the joined multicast state
    list<pair<pair<string, xorp_module_id>, IPvX> > joined_state
	= _joined_groups.joined_state();
    list<pair<pair<string, xorp_module_id>, IPvX> >::iterator iter;
    
    // Remove all the joined state and leave the groups that were joined by
    // a module that matches 'module_instance_name' and 'module_id'.
    for (iter = joined_state.begin(); iter != joined_state.end(); ++iter) {
	pair<string, xorp_module_id>& proto_state = (*iter).first;
	string& tmp_module_instance_name = proto_state.first;
	xorp_module_id tmp_module_id = proto_state.second;
	IPvX& group = (*iter).second;
	
	if ((module_instance_name != tmp_module_instance_name)
	    || (module_id != tmp_module_id))
	    continue;
	
	if (mfea_node().leave_multicast_group(module_instance_name,
					      module_id,
					      vif_index(),
					      group) == XORP_OK)
	    ret_value++;
    }
    
    return (ret_value);
}

/**
 * MfeaVif::JoinedGroups::add_multicast_group:
 * @module_instance_name: The module instance name of the protocol that
 * joins the group.
 * @module_id: The module ID of the protocol that joins the group.
 * @group: The address of the multicast group to add.
 * 
 * Add a group to the set of joined multicast groups.
 * 
 * Return value: %XORP_OK on success, otherwise %XORP_ERROR.
 **/
int
MfeaVif::JoinedGroups::add_multicast_group(const string& module_instance_name,
					   xorp_module_id module_id,
					   const IPvX& group)
{
    pair<string, xorp_module_id> my_pair(module_instance_name, module_id);
    
    if (find(_joined_state.begin(), _joined_state.end(),
	     pair<pair<string, xorp_module_id>, IPvX>(my_pair, group))
	!= _joined_state.end()) {
	return (XORP_ERROR);	// Already joined
    }
    
    // Add the pair of group address and module instance name
    _joined_state.push_back(pair<pair<string, xorp_module_id>, IPvX>(my_pair, group));
    
    //
    // Add the group address itself.
    // XXX: if the group was added already, the insert will silently fail.
    //
    _joined_multicast_groups.insert(group);
    
    return (XORP_OK);
}


/**
 * MfeaVif::JoinedGroups::delete_multicast_group:
 * @module_instance_name: The module instance name of the protocol that
 * leaves the group.
 * @module_id: The module ID of the protocol that leaves the group.
 * @group: The address of the multicast group to delete.
 * 
 * Delete a group from the set of joined multicast groups.
 * 
 * Return value: %XORP_OK on success, otherwise %XORP_ERROR.
 **/
int
MfeaVif::JoinedGroups::delete_multicast_group(const string& module_instance_name,
					      xorp_module_id module_id,
					      const IPvX& group)
{
    pair<string, xorp_module_id> my_pair(module_instance_name, module_id);
    
    list<pair<pair<string, xorp_module_id>, IPvX> >::iterator iter;
    
    iter = find(_joined_state.begin(), _joined_state.end(),
		pair<pair<string, xorp_module_id>, IPvX>(my_pair, group));
    if (iter == _joined_state.end())
	return (XORP_ERROR);	// Probably not added before
    
    _joined_state.erase(iter);
    
    //
    // Try to find if other instances have joined the same group
    //
    for (iter = _joined_state.begin(); iter != _joined_state.end(); ++iter) {
	pair<pair<string, xorp_module_id>, IPvX>& tmp_pair = *iter;
	if (tmp_pair.second == group)
	    return (XORP_OK);	// Group is still in use
    }
    
    // Last instance to join the group. Delete.
    if (_joined_multicast_groups.erase(group) > 0)
	return (XORP_OK);		// Deletion successful
    return (XORP_ERROR);		// Deletion failed (e.g. group not in the set)
}

/**
 * MfeaVif::JoinedGroups::has_multicast_group:
 * @group: The address of the multicast group to test.
 * 
 * Test if a multicast group belongs to the set of joined multicast groups.
 * 
 * Return value: true if @group belongs to the set of joined multicast groups,
 * otherwise false.
 **/
bool
MfeaVif::JoinedGroups::has_multicast_group(const IPvX& group) const
{
    return (_joined_multicast_groups.find(group)
	    != _joined_multicast_groups.end());
}

