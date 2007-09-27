// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2001-2007 International Computer Science Institute
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

#ident "$XORP: xorp/fea/data_plane/ifconfig/ifconfig_set_iphelper.cc,v 1.11 2007/09/25 23:07:55 pavlin Exp $"

#include "fea/fea_module.h"

#include "libxorp/xorp.h"
#include "libxorp/xlog.h"
#include "libxorp/debug.h"
#include "libxorp/win_io.h"
#include "libxorp/ipvx.hh"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_IPHLPAPI_H
#include <iphlpapi.h>
#endif

#include "fea/ifconfig.hh"
#include "fea/ifconfig_get.hh"

#include "ifconfig_set_iphelper.hh"


//
// Set information about network interfaces configuration with the
// underlying system.
//
// The mechanism to set the information is the IP Helper API for
// Windows (IPHLPAPI.DLL).
//

#ifdef HOST_OS_WINDOWS

IfConfigSetIPHelper::IfConfigSetIPHelper(FeaDataPlaneManager& fea_data_plane_manager)
    : IfConfigSet(fea_data_plane_manager)
{
}

IfConfigSetIPHelper::~IfConfigSetIPHelper()
{
    string error_msg;

    if (stop(error_msg) != XORP_OK) {
	XLOG_ERROR("Cannot stop the IP Helper API mechanism to set "
		   "information about network interfaces into the underlying "
		   "system: %s",
		   error_msg.c_str());
    }
}

int
IfConfigSetIPHelper::start(string& error_msg)
{
    UNUSED(error_msg);

    if (_is_running)
	return (XORP_OK);

    _is_running = true;

    return (XORP_OK);
}

int
IfConfigSetIPHelper::stop(string& error_msg)
{
    UNUSED(error_msg);

    if (! _is_running)
	return (XORP_OK);

    _is_running = false;

    return (XORP_OK);
}

bool
IfConfigSetIPHelper::is_discard_emulated(const IfTreeInterface& i) const
{
    UNUSED(i);

    return (false);
}

bool
IfConfigSetIPHelper::is_unreachable_emulated(const IfTreeInterface& i) const
{
    UNUSED(i);

    return (false);
}

int
IfConfigSetIPHelper::config_begin(string& error_msg)
{
    // XXX: nothing to do

    UNUSED(error_msg);

    return (XORP_OK);
}

int
IfConfigSetIPHelper::config_end(string& error_msg)
{
    // XXX: nothing to do

    UNUSED(error_msg);

    return (XORP_OK);
}

int
IfConfigSetIPHelper::config_interface_begin(const IfTreeInterface* pulled_ifp,
					    const IfTreeInterface& config_iface,
					    string& error_msg)
{
    if ((pulled_ifp == NULL) && config_iface.is_marked(IfTreeItem::DELETED)) {
	// Nothing to do: the interface has been deleted from the system
	return (XORP_OK);
    }

    //
    // Set the MTU
    //
    do {
	uint32_t mtu = config_iface.mtu();
	if ((mtu == 0) && (pulled_ifp != NULL))
	    mtu = pulled_ifp->mtu();
	if (mtu == 0)
	    break;
	if ((pulled_ifp == NULL) || (mtu != pulled_ifp->mtu())) {
	    error_msg = c_format("Cannot set the MTU to %u on "
				 "interface %s: method not supported",
				 mtu,
				 config_iface.ifname().c_str());
	    return (XORP_ERROR);
	}
	break;
    } while (false);

    //
    // Set the MAC address
    //
    do {
	Mac mac = config_iface.mac();
	if (mac.empty() && (pulled_ifp != NULL))
	    mac = pulled_ifp->mac();
	if (mac.empty())
	    break;
	if ((pulled_ifp == NULL) || (mac != pulled_ifp->mac())) {
	    error_msg = c_format("Cannot set the MAC address to %s "
				 "on interface %s: method not supported",
				 mac.str().c_str(),
				 config_iface.ifname().c_str());
	    return (XORP_ERROR):
	}
	break;
    } while (false);

    return (XORP_OK);
}

int
IfConfigSetIPHelper::config_interface_end(const IfTreeInterface* pulled_ifp,
					  const IfTreeInterface& config_iface,
					  string& error_msg)
{
    if ((pulled_ifp == NULL) && config_iface.is_marked(IfTreeItem::DELETED)) {
	// Nothing to do: the interface has been deleted from the system
	return (XORP_OK);
    }

    //
    // Set the interface flags
    //
    uint32_t interface_flags = config_iface.interface_flags();
    if (pulled_ifp != NULL)
	interface_flags = pulled_ifp->interface_flags();
    if ((pulled_ifp == NULL)
	|| (interface_flags != pulled_ifp->interface_flags())
	|| (config_iface.enabled() != pulled_ifp->enabled())) {
	if (set_interface_status(config_iface.ifname(),
				 config_iface.pif_index(), interface_flags,
				 config_iface.enabled(), error_msg)
	    != XORP_OK) {
	    return (XORP_ERROR);
	}
    }

    return (XORP_OK);
}

int
IfConfigSetIPHelper::config_vif_begin(const IfTreeInterface* pulled_ifp,
				      const IfTreeVif* pulled_vifp,
				      const IfTreeInterface& config_iface,
				      const IfTreeVif& config_vif,
				      string& error_msg)
{
    UNUSED(pulled_ifp);
    UNUSED(config_iface);
    UNUSED(error_msg);

    if ((pulled_vifp == NULL) && config_vif.is_marked(IfTreeItem::DELETED)) {
	// Nothing to do: the vif has been deleted from the system
	return (XORP_OK);
    }

    // XXX: nothing to do

    return (XORP_OK);
}

int
IfConfigSetIPHelper::config_vif_end(const IfTreeInterface* pulled_ifp,
				    const IfTreeVif* pulled_vifp,
				    const IfTreeInterface& config_iface,
				    const IfTreeVif& config_vif,
				    string& error_msg)
{
    UNUSED(pulled_ifp);

    if ((pulled_vifp == NULL) && config_vif.is_marked(IfTreeItem::DELETED)) {
	// Nothing to do: the vif has been deleted from the system
	return (XORP_OK);
    }

    //
    // XXX: If the interface name and vif name are different, then
    // they might have different status: the interface can be UP, while
    // the vif can be DOWN.
    //
    if (config_iface.ifname() != config_vif.vifname()) {
	//
	// Set the vif flags
	//
	uint32_t vif_flags = config_vif.vif_flags();
	if (pulled_vifp != NULL)
	    vif_flags = pulled_vifp->vif_flags();
	if ((pulled_vifp == NULL)
	    || (vif_flags != pulled_vifp->vif_flags())
	    || (config_vif.enabled() != pulled_vifp->enabled())) {
	    //
	    // XXX: The interface and vif status setting mechanism is
	    // equivalent for this platform.
	    //
	    if (set_interface_status(config_vif.vifname(),
				     config_vif.pif_index(), vif_flags,
				     config_vif.enabled(), error_msg)
		!= XORP_OK) {
		return (XORP_ERROR);
	    }
	}
    }

    return (XORP_OK);
}

int
IfConfigSetIPHelper::config_addr(const IfTreeInterface* pulled_ifp,
				 const IfTreeVif* pulled_vifp,
				 const IfTreeAddr4* pulled_addrp,
				 const IfTreeInterface& config_iface,
				 const IfTreeVif& config_vif,
				 const IfTreeAddr4& config_addr,
				 string& error_msg)
{
    bool is_deleted = false;

    UNUSED(pulled_ifp);
    UNUSED(pulled_vifp);

    if (! fea_data_plane_manager().have_ipv4()) {
	error_msg = "IPv4 is not supported";
	return (XORP_ERROR);
    }

    // XXX: Disabling an address is same as deleting it
    if (config_addr.is_marked(IfTreeItem::DELETED)
	|| (! config_addr.enabled())) {
	is_deleted = true;
    }

    //
    // Delete the address if marked for deletion
    //
    if (is_deleted) {
	if (pulled_addrp == NULL)
	    return (XORP_OK);		// XXX: nothing to delete

	if (delete_addr(config_iface.ifname(), config_vif.vifname(),
			config_vif.pif_index(), config_addr.addr(),
			config_addr.prefix_len(), error_msg)
	    != XORP_OK) {
	    return (XORP_ERROR);
	}

	return (XORP_OK);
    }

    //
    // Test whether a new address
    //
    do {
	if (pulled_addrp == NULL)
	    break;
	if (pulled_addrp->addr() != config_addr.addr())
	    break;
	if (pulled_addrp->broadcast() != config_addr.broadcast())
	    break;
	if (pulled_addrp->broadcast()
	    && (pulled_addrp->bcast() != config_addr.bcast())) {
	    break;
	}
	if (pulled_addrp->point_to_point() != config_addr.point_to_point())
	    break;
	if (pulled_addrp->point_to_point()
	    && (pulled_addrp->endpoint() != config_addr.endpoint())) {
	    break;
	}
	if (pulled_addrp->prefix_len() != config_addr.prefix_len())
	    break;

	// XXX: Same address, therefore ignore it
	return (XORP_OK);
    } while (false);

    //
    // Delete the old address if necessary
    //
    if (pulled_addrp != NULL) {
	if (delete_addr(config_iface.ifname(), config_vif.vifname(),
			config_vif.pif_index(), config_addr.addr(),
			config_addr.prefix_len(), error_msg)
	    != XORP_OK) {
	    return (XORP_ERROR);
	}
    }

    //
    // Add the address
    //
    if (add_addr(config_iface.ifname(), config_vif.vifname(),
		 config_vif.pif_index(), config_addr.addr(),
		 config_addr.prefix_len(),
		 config_addr.broadcast(), config_addr.bcast(),
		 config_addr.point_to_point(), config_addr.endpoint(),
		 error_msg)
	!= XORP_OK) {
	return (XORP_ERROR);
    }

    return (XORP_OK);
}

int
IfConfigSetIPHelper::config_addr(const IfTreeInterface* pulled_ifp,
				 const IfTreeVif* pulled_vifp,
				 const IfTreeAddr6* pulled_addrp,
				 const IfTreeInterface& config_iface,
				 const IfTreeVif& config_vif,
				 const IfTreeAddr6& config_addr,
				 string& error_msg)
{
    bool is_deleted = false;

    UNUSED(pulled_ifp);
    UNUSED(pulled_vifp);

    if (! fea_data_plane_manager().have_ipv6()) {
	error_msg = "IPv6 is not supported";
	return (XORP_ERROR);
    }

    // XXX: Disabling an address is same as deleting it
    if (config_addr.is_marked(IfTreeItem::DELETED)
	|| (! config_addr.enabled())) {
	is_deleted = true;
    }

    //
    // Delete the address if marked for deletion
    //
    if (is_deleted) {
	if (pulled_addrp == NULL)
	    return (XORP_OK);		// XXX: nothing to delete

	if (delete_addr(config_iface.ifname(), config_vif.vifname(),
			config_vif.pif_index(), config_addr.addr(),
			config_addr.prefix_len(), error_msg)
	    != XORP_OK) {
	    return (XORP_ERROR);
	}

	return (XORP_OK);
    }

    //
    // Test whether a new address
    //
    do {
	if (pulled_addrp == NULL)
	    break;
	if (pulled_addrp->addr() != config_addr.addr())
	    break;
	if (pulled_addrp->point_to_point() != config_addr.point_to_point())
	    break;
	if (pulled_addrp->point_to_point()
	    && (pulled_addrp->endpoint() != config_addr.endpoint())) {
	    break;
	}
	if (pulled_addrp->prefix_len() != config_addr.prefix_len())
	    break;

	// XXX: Same address, therefore ignore it
	return (XORP_OK);
    } while (false);

    //
    // Delete the old address if necessary
    //
    if (pulled_addrp != NULL) {
	if (delete_addr(config_iface.ifname(), config_vif.vifname(),
			config_vif.pif_index(), config_addr.addr(),
			config_addr.prefix_len(), error_msg)
	    != XORP_OK) {
	    return (XORP_ERROR);
	}
    }

    //
    // Add the address
    //
    if (add_addr(config_iface.ifname(), config_vif.vifname(),
		 config_vif.pif_index(), config_addr.addr(),
		 config_addr.prefix_len(),
		 config_addr.point_to_point(), config_addr.endpoint(),
		 error_msg)
	!= XORP_OK) {
	return (XORP_ERROR);
    }

    return (XORP_OK);
}

int
IfConfigSetIoctl::set_interface_status(const string& ifname,
				       uint32_t if_index,
				       uint32_t interface_flags,
				       bool is_enabled,
				       string& error_msg)
{
    // TODO: implement/test it!
#if 0
    MIB_IFROW ifrow;
    DWORD result;

    UNUSED(interface_flags);

    memset(&ifrow, 0, sizeof(ifrow));
    ifrow.dwIndex = if_index;

    result = GetIfEntry(&ifrow);
    if (result != NO_ERROR) {
	error_msg = c_format("Cannot obtain existing MIB_IFROW for "
			     "interface %s: error %d\n",
			     ifname.c_str(),
			     (int)result);
	return (XORP_ERROR);
    }
    if (is_enabled)
	ifrow.dwAdminStatus = MIB_IF_ADMIN_STATUS_UP;
    else
	ifrow.dwAdminStatus = MIB_IF_ADMIN_STATUS_DOWN;
    result = SetIfEntry(&ifrow);
    if (result != NO_ERROR) {
	error_msg = c_format("Cannot set administrative status of "
			     "interface %s: error %d\n",
			     ifname.c_str(),
			     (int)result);
	return (XORP_ERROR);
    }
#endif // 0

    return (XORP_OK);
}

int
IfConfigSetIPHelper::add_addr(const string& ifname, const string& vifname,
			      uint32_t if_index, const IPv4& addr,
			      uint32_t prefix_len, bool is_broadcast,
			      const IPv4& broadcast_addr,
			      bool is_point_to_point,
			      const IPv4& endpoint_addr,
			      string& error_msg)
{
    PMIB_IPADDRTABLE pAddrTable;
    DWORD       result, tries;
    ULONG       dwSize;
    IPAddr	ipaddr;
    IPMask	ipmask;

    addr.copy_out((uint8_t*)&ipaddr);
    IPv4 prefix_addr = IPv4::make_prefix(prefix_len);
    prefix_addr.copy_out((uint8_t*)&ipmask);

    tries = 0;
    result = ERROR_INSUFFICIENT_BUFFER;
    dwSize = sizeof(*pAddrTable) + (30 * sizeof(MIB_IPADDRROW));
    do {
        pAddrTable = (PMIB_IPADDRTABLE) ((tries == 0) ? malloc(dwSize) :
					 realloc(pAddrTable, dwSize));
        if (pAddrTable == NULL)
            break;
	result = GetIpAddrTable(pAddrTable, &dwSize, FALSE);
        if (pAddrTable == NULL)
            break;
    } while ((++tries < 3) || (result == ERROR_INSUFFICIENT_BUFFER));

    if (result != NO_ERROR) {
        XLOG_ERROR("GetIpAddrTable(): %s", win_strerror(result));
        if (pAddrTable != NULL)
            free(pAddrTable);
        return (XORP_OK);
    }

    for (unsigned int i = 0; i < pAddrTable->dwNumEntries; i++) {
	if (pAddrTable->table[i].dwAddr == ipaddr) {
	    XLOG_WARNING("IP address %s already exists on interface "
			 "with index %lu",
			 addr.str().c_str(),
			 pAddrTable->table[i].dwIndex);
            return (XORP_OK);
	}
    }

    ULONG	ntectx = 0, nteinst = 0;

    result = AddIPAddress(ipaddr, ipmask, if_index, &ntectx, &nteinst);
    if (result != NO_ERROR) {
	error_msg = c_format("Cannot add address '%s' "
			     "on interface '%s' vif '%s': error %d",
			     addr.str().c_str(), ifname.c_str(),
			     vifname.c_str(), (int)result);
	return (XORP_OK);
    }

    //
    // We can't delete IP addresses using IP Helper unless we cache the
    // returned NTE context. This means we can only delete addresses
    // which were created during the lifetime of the FEA.
    //
    // Also, these entries have to be keyed by the address we added,
    // so we can figure out which ntectx value to use when deleting it.
    //
    _nte_map.insert(make_pair(make_pair(if_index, (IPAddr)addr), ntectx));

    return (XORP_OK);
}

int
IfConfigSetIPHelper::delete_addr(const string& ifname, const string& vifname,
				 uint32_t if_index, const IPv4& addr,
				 uint32_t prefix_len, string& error_msg)
{
    map<pair<uint32_t, IPAddr>, ULONG>::iterator ii;

    UNUSED(prefix_len);

    ii = _nte_map.find(make_pair(if_index, (IPAddr)addr));
    if (ii == _nte_map.end()) {
	error_msg = c_format("Cannot delete address '%s' "
			     "on interface '%s' vif '%s': "
			     "address not found in internal table",
			     addr.str().c_str(), ifname.c_str(),
			     vifname.c_str());
	    return (XORP_ERROR);
    }

    ULONG ntectx = ii->second;
    DWORD result = DeleteIPAddress(ntectx);
    if (result != NO_ERROR) {
	error_msg = c_format("Cannot delete address '%s' "
			     "on interface '%s' vif '%s': error %d",
			     addr.str().c_str(), ifname.c_str(),
			     vifname.c_str(), (int)result);
	return (XORP_ERROR);
    }

    return (XORP_OK);
}

int
IfConfigSetIPHelper::add_addr(const string& ifname, const string& vifname,
			      uint32_t if_index, const IPv6& addr,
			      uint32_t prefix_len, bool is_point_to_point,
			      const IPv6& endpoint_addr, string& error_msg)
{
#ifndef HAVE_IPV6
    UNUSED(ifname);
    UNUSED(vifname);
    UNUSED(if_index);
    UNUSED(addr);
    UNUSED(prefix_len);
    UNUSED(is_point_to_point);
    UNUSED(endpoint_addr);

    error_msg = "IPv6 is not supported";

    return (XORP_ERROR);

#else // HAVE_IPV6

    //
    // No mechanism to add the address
    //
    UNUSED(ifname);
    UNUSED(vifname);
    UNUSED(if_index);
    UNUSED(addr);
    UNUSED(prefix_len);
    UNUSED(is_point_to_point);
    UNUSED(endpoint_addr);

    error_msg = c_format("No mechanism to add an IPv6 address "
			 "on an interface");
    return (XORP_ERROR);
#endif // HAVE_IPV6
}

int
IfConfigSetIPHelper::delete_addr(const string& ifname, const string& vifname,
				 uint32_t if_index, const IPv6& addr,
				 uint32_t prefix_len, string& error_msg)
{
#ifndef HAVE_IPV6
    UNUSED(ifname);
    UNUSED(vifname);
    UNUSED(if_index);
    UNUSED(addr);
    UNUSED(prefix_len);

    error_msg = "IPv6 is not supported";

    return (XORP_ERROR);

#else // HAVE_IPV6

    //
    // No mechanism to delete the address
    //
    UNUSED(ifname);
    UNUSED(vifname);
    UNUSED(if_index);
    UNUSED(addr);
    UNUSED(prefix_len);

    error_msg = c_format("No mechanism to delete an IPv6 address "
			 "on an interface");

    return (XORP_ERROR);
#endif
}

#endif // HOST_OS_WINDOWS
