/*
 * Copyright (c) 2001-2004 International Computer Science Institute
 * See LICENSE file for licensing, conditions, and warranties on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by 'clnt-gen'.
 *
 * $XORP: xorp/xrl/interfaces/static_routes_xif.hh,v 1.2 2004/03/30 03:22:04 pavlin Exp $
 */

#ifndef __XRL_INTERFACES_STATIC_ROUTES_XIF_HH__
#define __XRL_INTERFACES_STATIC_ROUTES_XIF_HH__

#undef XORP_LIBRARY_NAME
#define XORP_LIBRARY_NAME "XifStaticRoutes"

#include "libxorp/xlog.h"
#include "libxorp/callback.hh"

#include "libxipc/xrl.hh"
#include "libxipc/xrl_error.hh"
#include "libxipc/xrl_sender.hh"


class XrlStaticRoutesV0p1Client {
public:
    XrlStaticRoutesV0p1Client(XrlSender* s) : _sender(s) {}
    virtual ~XrlStaticRoutesV0p1Client() {}

    typedef XorpCallback1<void, const XrlError&>::RefPtr EnableStaticRoutesCB;
    /**
     *  Send Xrl intended to:
     *
     *  Enable/disable/start/stop StaticRoutes.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param enable if true, then enable StaticRoutes, otherwise disable it.
     */
    bool send_enable_static_routes(
	const char*	target_name,
	const bool&	enable,
	const EnableStaticRoutesCB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr StartStaticRoutesCB;

    bool send_start_static_routes(
	const char*	target_name,
	const StartStaticRoutesCB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr StopStaticRoutesCB;

    bool send_stop_static_routes(
	const char*	target_name,
	const StopStaticRoutesCB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr AddRoute4CB;
    /**
     *  Send Xrl intended to:
     *
     *  Add/replace/delete a static route.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param unicast if true, then the route would be used for unicast
     *  routing.
     *
     *  @param multicast if true, then the route would be used in the MRIB
     *  (Multicast Routing Information Base) for multicast purpose (e.g.,
     *  computing the Reverse-Path Forwarding information).
     *
     *  @param network the network address prefix this route applies to.
     *
     *  @param nexthop the address of the next-hop router for this route.
     *
     *  @param metric the metric distance for this route.
     */
    bool send_add_route4(
	const char*	target_name,
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
	const bool&	unicast,
	const bool&	multicast,
	const IPv4Net&	network,
	const DeleteRoute4CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr DeleteRoute6CB;

    bool send_delete_route6(
	const char*	target_name,
	const bool&	unicast,
	const bool&	multicast,
	const IPv6Net&	network,
	const DeleteRoute6CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr AddInterfaceRoute4CB;
    /**
     *  Send Xrl intended to:
     *
     *  Add/replace a static route by explicitly specifying the network
     *  interface toward the destination.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param unicast if true, then the route would be used for unicast
     *  routing.
     *
     *  @param multicast if true, then the route would be used in the MRIB
     *  (Multicast Routing Information Base) for multicast purpose (e.g.,
     *  computing the Reverse-Path Forwarding information).
     *
     *  @param network the network address prefix this route applies to.
     *
     *  @param nexthop the address of the next-hop router for this route.
     *
     *  @param ifname of the name of the physical interface toward the
     *  destination.
     *
     *  @param vifname of the name of the virtual interface toward the
     *  destination.
     *
     *  @param metric the metric distance for this route.
     */
    bool send_add_interface_route4(
	const char*	target_name,
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
	const bool&	unicast,
	const bool&	multicast,
	const IPv6Net&	network,
	const IPv6&	nexthop,
	const string&	ifname,
	const string&	vifname,
	const uint32_t&	metric,
	const ReplaceInterfaceRoute6CB&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr EnableLogTraceAllCB;
    /**
     *  Send Xrl intended to:
     *
     *  Enable/disable the StaticRoutes trace log for all operations.
     *
     *  @param tgt_name Xrl Target name
     *
     *  @param enable if true, then enable the trace log, otherwise disable it.
     */
    bool send_enable_log_trace_all(
	const char*	target_name,
	const bool&	enable,
	const EnableLogTraceAllCB&	cb
    );

protected:
    XrlSender* _sender;

private:
    void unmarshall_enable_static_routes(
	const XrlError&	e,
	XrlArgs*	a,
	EnableStaticRoutesCB		cb
    );

    void unmarshall_start_static_routes(
	const XrlError&	e,
	XrlArgs*	a,
	StartStaticRoutesCB		cb
    );

    void unmarshall_stop_static_routes(
	const XrlError&	e,
	XrlArgs*	a,
	StopStaticRoutesCB		cb
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

    void unmarshall_enable_log_trace_all(
	const XrlError&	e,
	XrlArgs*	a,
	EnableLogTraceAllCB		cb
    );

};

#endif /* __XRL_INTERFACES_STATIC_ROUTES_XIF_HH__ */
