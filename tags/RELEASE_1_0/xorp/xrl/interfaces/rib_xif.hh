/*
 * Copyright (c) 2001-2004 International Computer Science Institute
 * See LICENSE file for licensing, conditions, and warranties on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by 'clnt-gen'.
 *
 * $XORP: xorp/xrl/interfaces/rib_xif.hh,v 1.22 2004/05/21 00:02:55 pavlin Exp $
 */

#ifndef __XRL_INTERFACES_RIB_XIF_HH__
#define __XRL_INTERFACES_RIB_XIF_HH__

#undef XORP_LIBRARY_NAME
#define XORP_LIBRARY_NAME "XifRib"

#include "libxorp/xlog.h"
#include "libxorp/callback.hh"

#include "libxipc/xrl.hh"
#include "libxipc/xrl_error.hh"
#include "libxipc/xrl_sender.hh"


class XrlRibV0p1Client {
public:
    XrlRibV0p1Client(XrlSender* s) : _sender(s) {}
    virtual ~XrlRibV0p1Client() {}

    typedef XorpCallback1<void, const XrlError&>::RefPtr EnableRibCB;
    /**
     *  Send Xrl intended to:
     *
     *  Enable/disable/start/stop RIB.
     *
     *  @param tgt_name Xrl Target name
     */
    bool send_enable_rib(
	const char*	target_name,
	const EnableRibCB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr DisableRibCB;

    bool send_disable_rib(
	const char*	target_name,
	const DisableRibCB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr StartRibCB;

    bool send_start_rib(
	const char*	target_name,
	const StartRibCB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr StopRibCB;

    bool send_stop_rib(
	const char*	target_name,
	const StopRibCB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr AddRibClient4CB;
    /**
     *  Send Xrl intended to:
     *
     *  Add/delete/enable/disable a RIB client. Add/delete/enable/disable a RIB
     *  client for a given target name, address family, and unicast/multicast
     *  flags.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param target_name the target name of the RIB client.
     *
     *  @param unicast true if a client for the unicast RIB.
     *
     *  @param multicast true if a client for the multicast RIB.
     */
    bool send_add_rib_client4(
	const char*	target_name,
	const string&	target_name,
	const bool&	unicast,
	const bool&	multicast,
	const AddRibClient4CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr AddRibClient6CB;

    bool send_add_rib_client6(
	const char*	target_name,
	const string&	target_name,
	const bool&	unicast,
	const bool&	multicast,
	const AddRibClient6CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr DeleteRibClient4CB;

    bool send_delete_rib_client4(
	const char*	target_name,
	const string&	target_name,
	const bool&	unicast,
	const bool&	multicast,
	const DeleteRibClient4CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr DeleteRibClient6CB;

    bool send_delete_rib_client6(
	const char*	target_name,
	const string&	target_name,
	const bool&	unicast,
	const bool&	multicast,
	const DeleteRibClient6CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr EnableRibClient4CB;

    bool send_enable_rib_client4(
	const char*	target_name,
	const string&	target_name,
	const bool&	unicast,
	const bool&	multicast,
	const EnableRibClient4CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr EnableRibClient6CB;

    bool send_enable_rib_client6(
	const char*	target_name,
	const string&	target_name,
	const bool&	unicast,
	const bool&	multicast,
	const EnableRibClient6CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr DisableRibClient4CB;

    bool send_disable_rib_client4(
	const char*	target_name,
	const string&	target_name,
	const bool&	unicast,
	const bool&	multicast,
	const DisableRibClient4CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr DisableRibClient6CB;

    bool send_disable_rib_client6(
	const char*	target_name,
	const string&	target_name,
	const bool&	unicast,
	const bool&	multicast,
	const DisableRibClient6CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr MakeErrorsFatalCB;
    /**
     *  Send Xrl intended to:
     *
     *  Make errors fatal; used to detect errors we'd normally mask
     *
     *  @param tgt_name Xrl Target name
     */
    bool send_make_errors_fatal(
	const char*	target_name,
	const MakeErrorsFatalCB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr AddIgpTable4CB;
    /**
     *  Send Xrl intended to:
     *
     *  Add/delete an IGP or EGP table.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param protocol the name of the protocol.
     *
     *  @param target_class the target class of the protocol.
     *
     *  @param target_instance the target instance of the protocol.
     *
     *  @param unicast true if the table is for the unicast RIB.
     *
     *  @param multicast true if the table is for the multicast RIB.
     */
    bool send_add_igp_table4(
	const char*	target_name,
	const string&	protocol,
	const string&	target_class,
	const string&	target_instance,
	const bool&	unicast,
	const bool&	multicast,
	const AddIgpTable4CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr AddIgpTable6CB;

    bool send_add_igp_table6(
	const char*	target_name,
	const string&	protocol,
	const string&	target_class,
	const string&	target_instance,
	const bool&	unicast,
	const bool&	multicast,
	const AddIgpTable6CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr DeleteIgpTable4CB;

    bool send_delete_igp_table4(
	const char*	target_name,
	const string&	protocol,
	const string&	target_class,
	const string&	target_instance,
	const bool&	unicast,
	const bool&	multicast,
	const DeleteIgpTable4CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr DeleteIgpTable6CB;

    bool send_delete_igp_table6(
	const char*	target_name,
	const string&	protocol,
	const string&	target_class,
	const string&	target_instance,
	const bool&	unicast,
	const bool&	multicast,
	const DeleteIgpTable6CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr AddEgpTable4CB;

    bool send_add_egp_table4(
	const char*	target_name,
	const string&	protocol,
	const string&	target_class,
	const string&	target_instance,
	const bool&	unicast,
	const bool&	multicast,
	const AddEgpTable4CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr AddEgpTable6CB;

    bool send_add_egp_table6(
	const char*	target_name,
	const string&	protocol,
	const string&	target_class,
	const string&	target_instance,
	const bool&	unicast,
	const bool&	multicast,
	const AddEgpTable6CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr DeleteEgpTable4CB;

    bool send_delete_egp_table4(
	const char*	target_name,
	const string&	protocol,
	const string&	target_class,
	const string&	target_instance,
	const bool&	unicast,
	const bool&	multicast,
	const DeleteEgpTable4CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr DeleteEgpTable6CB;

    bool send_delete_egp_table6(
	const char*	target_name,
	const string&	protocol,
	const string&	target_class,
	const string&	target_instance,
	const bool&	unicast,
	const bool&	multicast,
	const DeleteEgpTable6CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr AddRoute4CB;
    /**
     *  Send Xrl intended to:
     *
     *  Add/replace/delete a route.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param protocol the name of the protocol this route comes from.
     *
     *  @param unicast true if the route is for the unicast RIB.
     *
     *  @param multicast true if the route is for the multicast RIB.
     *
     *  @param network the network address prefix of the route.
     *
     *  @param nexthop the address of the next-hop router toward the
     *  destination.
     *
     *  @param metric the routing metric.
     */
    bool send_add_route4(
	const char*	target_name,
	const string&	protocol,
	const bool&	unicast,
	const bool&	multicast,
	const IPv4Net&	network,
	const IPv4&	nexthop,
	const uint32_t&	metric,
	const AddRoute4CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr AddRoute6CB;

    bool send_add_route6(
	const char*	target_name,
	const string&	protocol,
	const bool&	unicast,
	const bool&	multicast,
	const IPv6Net&	network,
	const IPv6&	nexthop,
	const uint32_t&	metric,
	const AddRoute6CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr ReplaceRoute4CB;

    bool send_replace_route4(
	const char*	target_name,
	const string&	protocol,
	const bool&	unicast,
	const bool&	multicast,
	const IPv4Net&	network,
	const IPv4&	nexthop,
	const uint32_t&	metric,
	const ReplaceRoute4CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr ReplaceRoute6CB;

    bool send_replace_route6(
	const char*	target_name,
	const string&	protocol,
	const bool&	unicast,
	const bool&	multicast,
	const IPv6Net&	network,
	const IPv6&	nexthop,
	const uint32_t&	metric,
	const ReplaceRoute6CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr DeleteRoute4CB;

    bool send_delete_route4(
	const char*	target_name,
	const string&	protocol,
	const bool&	unicast,
	const bool&	multicast,
	const IPv4Net&	network,
	const DeleteRoute4CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr DeleteRoute6CB;

    bool send_delete_route6(
	const char*	target_name,
	const string&	protocol,
	const bool&	unicast,
	const bool&	multicast,
	const IPv6Net&	network,
	const DeleteRoute6CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr AddInterfaceRoute4CB;
    /**
     *  Send Xrl intended to:
     *
     *  Add/replace a route by explicitly specifying the network interface
     *  toward the destination.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param protocol the name of the protocol this route comes from.
     *
     *  @param unicast true if the route is for the unicast RIB.
     *
     *  @param multicast true if the route is for the multicast RIB.
     *
     *  @param network the network address prefix of the route.
     *
     *  @param nexthop the address of the next-hop router toward the
     *  destination.
     *
     *  @param ifname of the name of the physical interface toward the
     *  destination.
     *
     *  @param vifname of the name of the virtual interface toward the
     *  destination.
     *
     *  @param metric the routing metric.
     */
    bool send_add_interface_route4(
	const char*	target_name,
	const string&	protocol,
	const bool&	unicast,
	const bool&	multicast,
	const IPv4Net&	network,
	const IPv4&	nexthop,
	const string&	ifname,
	const string&	vifname,
	const uint32_t&	metric,
	const AddInterfaceRoute4CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr AddInterfaceRoute6CB;

    bool send_add_interface_route6(
	const char*	target_name,
	const string&	protocol,
	const bool&	unicast,
	const bool&	multicast,
	const IPv6Net&	network,
	const IPv6&	nexthop,
	const string&	ifname,
	const string&	vifname,
	const uint32_t&	metric,
	const AddInterfaceRoute6CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr ReplaceInterfaceRoute4CB;

    bool send_replace_interface_route4(
	const char*	target_name,
	const string&	protocol,
	const bool&	unicast,
	const bool&	multicast,
	const IPv4Net&	network,
	const IPv4&	nexthop,
	const string&	ifname,
	const string&	vifname,
	const uint32_t&	metric,
	const ReplaceInterfaceRoute4CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr ReplaceInterfaceRoute6CB;

    bool send_replace_interface_route6(
	const char*	target_name,
	const string&	protocol,
	const bool&	unicast,
	const bool&	multicast,
	const IPv6Net&	network,
	const IPv6&	nexthop,
	const string&	ifname,
	const string&	vifname,
	const uint32_t&	metric,
	const ReplaceInterfaceRoute6CB&	cb
    );

    typedef XorpCallback2<void, const XrlError&, const IPv4*>::RefPtr LookupRoute4CB;
    /**
     *  Send Xrl intended to:
     *
     *  Lookup nexthop.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param addr address to lookup.
     *
     *  @param unicast look in unicast RIB.
     *
     *  @param multicast look in multicast RIB.
     */
    bool send_lookup_route4(
	const char*	target_name,
	const IPv4&	addr,
	const bool&	unicast,
	const bool&	multicast,
	const LookupRoute4CB&	cb
    );

    typedef XorpCallback2<void, const XrlError&, const IPv6*>::RefPtr LookupRoute6CB;
    /**
     *  Send Xrl intended to:
     *
     *  Lookup nexthop.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param addr address to lookup.
     *
     *  @param unicast look in unicast RIB.
     *
     *  @param multicast look in multicast RIB.
     */
    bool send_lookup_route6(
	const char*	target_name,
	const IPv6&	addr,
	const bool&	unicast,
	const bool&	multicast,
	const LookupRoute6CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr NewVifCB;
    /**
     *  Send Xrl intended to:
     *
     *  Add a vif or a vif address to the RIB. This interface should be used
     *  only for testing purpose.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param name the name of the vif.
     */
    bool send_new_vif(
	const char*	target_name,
	const string&	name,
	const NewVifCB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr AddVifAddr4CB;
    /**
     *  Send Xrl intended to:
     *
     *  Add a vif address to the RIB. This interface should be used only for
     *  testing purpose.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param name the name of the vif.
     *
     *  @param addr the address to add.
     *
     *  @param subnet the subnet address to add.
     */
    bool send_add_vif_addr4(
	const char*	target_name,
	const string&	name,
	const IPv4&	addr,
	const IPv4Net&	subnet,
	const AddVifAddr4CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr AddVifAddr6CB;

    bool send_add_vif_addr6(
	const char*	target_name,
	const string&	name,
	const IPv6&	addr,
	const IPv6Net&	subnet,
	const AddVifAddr6CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr RedistEnable4CB;
    /**
     *  Send Xrl intended to:
     *
     *  Enable route redistribution from one routing protocol to another.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param to_xrl_target the XRL Target instance name of the caller. The
     *  caller must implement redist4/0.1.
     *
     *  @param from_protocol the name of the routing process routes are to be
     *  redistributed from.
     *
     *  @param unicast enable for unicast RIBs matching from and to.
     *
     *  @param multicast enable for multicast RIBs matching from and to.
     *
     *  @param cookie a text value passed back to creator in each call from the
     *  RIB. This allows creators to identity the source of updates it receives
     *  through the redist4/0.1 interface.
     */
    bool send_redist_enable4(
	const char*	target_name,
	const string&	to_xrl_target,
	const string&	from_protocol,
	const bool&	unicast,
	const bool&	multicast,
	const string&	cookie,
	const RedistEnable4CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr RedistEnable6CB;
    /**
     *  Send Xrl intended to:
     *
     *  Enable route redistribution from one routing protocol to another.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param to_xrl_target the XRL Target instance name of the caller. The
     *  caller must implement redist6/0.1.
     *
     *  @param from_protocol the name of the routing process routes are to be
     *  redistributed from.
     *
     *  @param unicast enable for unicast RIBs matching from and to.
     *
     *  @param multicast enable for multicast RIBs matching from and to.
     *
     *  @param cookie a text value passed back to creator in each call from the
     *  RIB. This allows creators to identity the source of updates it receives
     *  through the redist6/0.1 interface.
     */
    bool send_redist_enable6(
	const char*	target_name,
	const string&	to_xrl_target,
	const string&	from_protocol,
	const bool&	unicast,
	const bool&	multicast,
	const string&	cookie,
	const RedistEnable6CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr RedistDisable4CB;
    /**
     *  Send Xrl intended to:
     *
     *  Disable route redistribution from one routing protocol to another.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param to_xrl_target the XRL Target instance name of the caller. The
     *  caller must implement redist4/0.1 and previously called redist_enable4.
     *
     *  @param unicast disable for unicast RIBs matching from and to.
     *
     *  @param multicast disable for multicast RIBs matching from and to.
     */
    bool send_redist_disable4(
	const char*	target_name,
	const string&	to_xrl_target,
	const string&	from_protocol,
	const bool&	unicast,
	const bool&	multicast,
	const string&	cookie,
	const RedistDisable4CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr RedistDisable6CB;
    /**
     *  Send Xrl intended to:
     *
     *  Disable route redistribution from one routing protocol to another.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param to_xrl_target the XRL Target instance name of the caller. The
     *  caller must implement redist6/0.1 and previously called redist_enable6.
     *
     *  @param unicast disable for unicast RIBs matching from and to.
     *
     *  @param multicast disable for multicast RIBs matching from and to.
     */
    bool send_redist_disable6(
	const char*	target_name,
	const string&	to_xrl_target,
	const string&	from_protocol,
	const bool&	unicast,
	const bool&	multicast,
	const string&	cookie,
	const RedistDisable6CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr RedistTransactionEnable4CB;
    /**
     *  Send Xrl intended to:
     *
     *  Enable transaction-based route redistribution from one routing protocol
     *  to another.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param to_xrl_target the XRL Target instance name of the caller. The
     *  caller must implement redist_transaction4/0.1.
     *
     *  @param from_protocol the name of the routing process routes are to be
     *  redistributed from.
     *
     *  @param unicast enable for unicast RIBs matching from and to.
     *
     *  @param multicast enable for multicast RIBs matching from and to.
     *
     *  @param cookie a text value passed back to creator in each call from the
     *  RIB. This allows creators to identity the source of updates it receives
     *  through the redist_transaction4/0.1 interface.
     */
    bool send_redist_transaction_enable4(
	const char*	target_name,
	const string&	to_xrl_target,
	const string&	from_protocol,
	const bool&	unicast,
	const bool&	multicast,
	const string&	cookie,
	const RedistTransactionEnable4CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr RedistTransactionEnable6CB;
    /**
     *  Send Xrl intended to:
     *
     *  Enable transaction-based route redistribution from one routing protocol
     *  to another.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param to_xrl_target the XRL Target instance name of the caller. The
     *  caller must implement redist_transaction6/0.1.
     *
     *  @param from_protocol the name of the routing process routes are to be
     *  redistributed from.
     *
     *  @param unicast enable for unicast RIBs matching from and to.
     *
     *  @param multicast enable for multicast RIBs matching from and to.
     *
     *  @param cookie a text value passed back to creator in each call from the
     *  RIB. This allows creators to identity the source of updates it receives
     *  through the redist_transaction6/0.1 interface.
     */
    bool send_redist_transaction_enable6(
	const char*	target_name,
	const string&	to_xrl_target,
	const string&	from_protocol,
	const bool&	unicast,
	const bool&	multicast,
	const string&	cookie,
	const RedistTransactionEnable6CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr RedistTransactionDisable4CB;
    /**
     *  Send Xrl intended to:
     *
     *  Disable transaction-based route redistribution from one routing
     *  protocol to another.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param to_xrl_target the XRL Target instance name of the caller. The
     *  caller must implement redist_transaction4/0.1 and previously called
     *  redist_transaction_enable4.
     *
     *  @param unicast disable for unicast RIBs matching from and to.
     *
     *  @param multicast disable for multicast RIBs matching from and to.
     */
    bool send_redist_transaction_disable4(
	const char*	target_name,
	const string&	to_xrl_target,
	const string&	from_protocol,
	const bool&	unicast,
	const bool&	multicast,
	const string&	cookie,
	const RedistTransactionDisable4CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr RedistTransactionDisable6CB;
    /**
     *  Send Xrl intended to:
     *
     *  Disable transaction-based route redistribution from one routing
     *  protocol to another.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param to_xrl_target the XRL Target instance name of the caller. The
     *  caller must implement redist_transaction6/0.1 and previously called
     *  redist_transaction_enable6.
     *
     *  @param unicast disable for unicast RIBs matching from and to.
     *
     *  @param multicast disable for multicast RIBs matching from and to.
     */
    bool send_redist_transaction_disable6(
	const char*	target_name,
	const string&	to_xrl_target,
	const string&	from_protocol,
	const bool&	unicast,
	const bool&	multicast,
	const string&	cookie,
	const RedistTransactionDisable6CB&	cb
    );

    typedef XorpCallback7<void, const XrlError&, const bool*, const IPv4*, const uint32_t*, const uint32_t*, const IPv4*, const uint32_t*>::RefPtr RegisterInterest4CB;
    /**
     *  Send Xrl intended to:
     *
     *  Register an interest in a route.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param target the name of the XRL module to notify when the information
     *  returned by this call becomes invalid.
     *
     *  @param addr address of interest.
     */
    bool send_register_interest4(
	const char*	target_name,
	const string&	target,
	const IPv4&	addr,
	const RegisterInterest4CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr DeregisterInterest4CB;
    /**
     *  Send Xrl intended to:
     *
     *  De-register an interest in a route.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param target the name of the XRL module that registered the interest.
     *
     *  @param addr the address of the previous registered interest. addr
     *  should be the base address of the add/prefix_len subnet.
     *
     *  @param prefix_len the prefix length of the registered interest, as
     *  given in the response from register_interest.
     */
    bool send_deregister_interest4(
	const char*	target_name,
	const string&	target,
	const IPv4&	addr,
	const uint32_t&	prefix_len,
	const DeregisterInterest4CB&	cb
    );

    typedef XorpCallback7<void, const XrlError&, const bool*, const IPv6*, const uint32_t*, const uint32_t*, const IPv6*, const uint32_t*>::RefPtr RegisterInterest6CB;
    /**
     *  Send Xrl intended to:
     *
     *  Register an interest in a route.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param target the name of the XRL module to notify when the information
     *  returned by this call becomes invalid.
     *
     *  @param addr address of interest.
     */
    bool send_register_interest6(
	const char*	target_name,
	const string&	target,
	const IPv6&	addr,
	const RegisterInterest6CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr DeregisterInterest6CB;
    /**
     *  Send Xrl intended to:
     *
     *  De-register an interest in a route.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param target the name of the XRL module that registered the interest.
     *
     *  @param addr the address of the previous registered interest. addr
     *  should be the base address of the add/prefix_len subnet.
     *
     *  @param prefix_len the prefix length of the registered interest, as
     *  given in the response from register_interest.
     */
    bool send_deregister_interest6(
	const char*	target_name,
	const string&	target,
	const IPv6&	addr,
	const uint32_t&	prefix_len,
	const DeregisterInterest6CB&	cb
    );

protected:
    XrlSender* _sender;

private:
    void unmarshall_enable_rib(
	const XrlError&	e,
	XrlArgs*	a,
	EnableRibCB		cb
    );

    void unmarshall_disable_rib(
	const XrlError&	e,
	XrlArgs*	a,
	DisableRibCB		cb
    );

    void unmarshall_start_rib(
	const XrlError&	e,
	XrlArgs*	a,
	StartRibCB		cb
    );

    void unmarshall_stop_rib(
	const XrlError&	e,
	XrlArgs*	a,
	StopRibCB		cb
    );

    void unmarshall_add_rib_client4(
	const XrlError&	e,
	XrlArgs*	a,
	AddRibClient4CB		cb
    );

    void unmarshall_add_rib_client6(
	const XrlError&	e,
	XrlArgs*	a,
	AddRibClient6CB		cb
    );

    void unmarshall_delete_rib_client4(
	const XrlError&	e,
	XrlArgs*	a,
	DeleteRibClient4CB		cb
    );

    void unmarshall_delete_rib_client6(
	const XrlError&	e,
	XrlArgs*	a,
	DeleteRibClient6CB		cb
    );

    void unmarshall_enable_rib_client4(
	const XrlError&	e,
	XrlArgs*	a,
	EnableRibClient4CB		cb
    );

    void unmarshall_enable_rib_client6(
	const XrlError&	e,
	XrlArgs*	a,
	EnableRibClient6CB		cb
    );

    void unmarshall_disable_rib_client4(
	const XrlError&	e,
	XrlArgs*	a,
	DisableRibClient4CB		cb
    );

    void unmarshall_disable_rib_client6(
	const XrlError&	e,
	XrlArgs*	a,
	DisableRibClient6CB		cb
    );

    void unmarshall_make_errors_fatal(
	const XrlError&	e,
	XrlArgs*	a,
	MakeErrorsFatalCB		cb
    );

    void unmarshall_add_igp_table4(
	const XrlError&	e,
	XrlArgs*	a,
	AddIgpTable4CB		cb
    );

    void unmarshall_add_igp_table6(
	const XrlError&	e,
	XrlArgs*	a,
	AddIgpTable6CB		cb
    );

    void unmarshall_delete_igp_table4(
	const XrlError&	e,
	XrlArgs*	a,
	DeleteIgpTable4CB		cb
    );

    void unmarshall_delete_igp_table6(
	const XrlError&	e,
	XrlArgs*	a,
	DeleteIgpTable6CB		cb
    );

    void unmarshall_add_egp_table4(
	const XrlError&	e,
	XrlArgs*	a,
	AddEgpTable4CB		cb
    );

    void unmarshall_add_egp_table6(
	const XrlError&	e,
	XrlArgs*	a,
	AddEgpTable6CB		cb
    );

    void unmarshall_delete_egp_table4(
	const XrlError&	e,
	XrlArgs*	a,
	DeleteEgpTable4CB		cb
    );

    void unmarshall_delete_egp_table6(
	const XrlError&	e,
	XrlArgs*	a,
	DeleteEgpTable6CB		cb
    );

    void unmarshall_add_route4(
	const XrlError&	e,
	XrlArgs*	a,
	AddRoute4CB		cb
    );

    void unmarshall_add_route6(
	const XrlError&	e,
	XrlArgs*	a,
	AddRoute6CB		cb
    );

    void unmarshall_replace_route4(
	const XrlError&	e,
	XrlArgs*	a,
	ReplaceRoute4CB		cb
    );

    void unmarshall_replace_route6(
	const XrlError&	e,
	XrlArgs*	a,
	ReplaceRoute6CB		cb
    );

    void unmarshall_delete_route4(
	const XrlError&	e,
	XrlArgs*	a,
	DeleteRoute4CB		cb
    );

    void unmarshall_delete_route6(
	const XrlError&	e,
	XrlArgs*	a,
	DeleteRoute6CB		cb
    );

    void unmarshall_add_interface_route4(
	const XrlError&	e,
	XrlArgs*	a,
	AddInterfaceRoute4CB		cb
    );

    void unmarshall_add_interface_route6(
	const XrlError&	e,
	XrlArgs*	a,
	AddInterfaceRoute6CB		cb
    );

    void unmarshall_replace_interface_route4(
	const XrlError&	e,
	XrlArgs*	a,
	ReplaceInterfaceRoute4CB		cb
    );

    void unmarshall_replace_interface_route6(
	const XrlError&	e,
	XrlArgs*	a,
	ReplaceInterfaceRoute6CB		cb
    );

    void unmarshall_lookup_route4(
	const XrlError&	e,
	XrlArgs*	a,
	LookupRoute4CB		cb
    );

    void unmarshall_lookup_route6(
	const XrlError&	e,
	XrlArgs*	a,
	LookupRoute6CB		cb
    );

    void unmarshall_new_vif(
	const XrlError&	e,
	XrlArgs*	a,
	NewVifCB		cb
    );

    void unmarshall_add_vif_addr4(
	const XrlError&	e,
	XrlArgs*	a,
	AddVifAddr4CB		cb
    );

    void unmarshall_add_vif_addr6(
	const XrlError&	e,
	XrlArgs*	a,
	AddVifAddr6CB		cb
    );

    void unmarshall_redist_enable4(
	const XrlError&	e,
	XrlArgs*	a,
	RedistEnable4CB		cb
    );

    void unmarshall_redist_enable6(
	const XrlError&	e,
	XrlArgs*	a,
	RedistEnable6CB		cb
    );

    void unmarshall_redist_disable4(
	const XrlError&	e,
	XrlArgs*	a,
	RedistDisable4CB		cb
    );

    void unmarshall_redist_disable6(
	const XrlError&	e,
	XrlArgs*	a,
	RedistDisable6CB		cb
    );

    void unmarshall_redist_transaction_enable4(
	const XrlError&	e,
	XrlArgs*	a,
	RedistTransactionEnable4CB		cb
    );

    void unmarshall_redist_transaction_enable6(
	const XrlError&	e,
	XrlArgs*	a,
	RedistTransactionEnable6CB		cb
    );

    void unmarshall_redist_transaction_disable4(
	const XrlError&	e,
	XrlArgs*	a,
	RedistTransactionDisable4CB		cb
    );

    void unmarshall_redist_transaction_disable6(
	const XrlError&	e,
	XrlArgs*	a,
	RedistTransactionDisable6CB		cb
    );

    void unmarshall_register_interest4(
	const XrlError&	e,
	XrlArgs*	a,
	RegisterInterest4CB		cb
    );

    void unmarshall_deregister_interest4(
	const XrlError&	e,
	XrlArgs*	a,
	DeregisterInterest4CB		cb
    );

    void unmarshall_register_interest6(
	const XrlError&	e,
	XrlArgs*	a,
	RegisterInterest6CB		cb
    );

    void unmarshall_deregister_interest6(
	const XrlError&	e,
	XrlArgs*	a,
	DeregisterInterest6CB		cb
    );

};

#endif /* __XRL_INTERFACES_RIB_XIF_HH__ */
