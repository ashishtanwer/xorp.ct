/*
 * Copyright (c) 2001-2003 International Computer Science Institute
 * See LICENSE file for licensing, conditions, and warranties on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by 'clnt-gen'.
 */

#ident "$XORP: xorp/xrl/interfaces/rib_client_xif.cc,v 1.6 2003/03/29 19:03:12 pavlin Exp $"

#include "rib_client_xif.hh"

bool
XrlRibClientV0p1Client::send_route_info_changed4(
	const char*	the_tgt, 
	const IPv4&	addr, 
	const uint32_t&	prefix_len, 
	const IPv4&	nexthop, 
	const uint32_t&	metric, 
	const uint32_t&	admin_distance, 
	const string&	protocol_origin, 
	const CB0&	cb
)
{
    Xrl x(the_tgt, "rib_client/0.1/route_info_changed4");
    x.args().add("addr", addr);
    x.args().add("prefix_len", prefix_len);
    x.args().add("nexthop", nexthop);
    x.args().add("metric", metric);
    x.args().add("admin_distance", admin_distance);
    x.args().add("protocol_origin", protocol_origin);
    return _sender->send(x, callback(this, &XrlRibClientV0p1Client::unmarshall0, cb));
}


/* Unmarshall route_info_changed4 */
void
XrlRibClientV0p1Client::unmarshall0(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB0		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibClientV0p1Client::send_route_info_changed6(
	const char*	the_tgt, 
	const IPv6&	addr, 
	const uint32_t&	prefix_len, 
	const IPv6&	nexthop, 
	const uint32_t&	metric, 
	const uint32_t&	admin_distance, 
	const string&	protocol_origin, 
	const CB1&	cb
)
{
    Xrl x(the_tgt, "rib_client/0.1/route_info_changed6");
    x.args().add("addr", addr);
    x.args().add("prefix_len", prefix_len);
    x.args().add("nexthop", nexthop);
    x.args().add("metric", metric);
    x.args().add("admin_distance", admin_distance);
    x.args().add("protocol_origin", protocol_origin);
    return _sender->send(x, callback(this, &XrlRibClientV0p1Client::unmarshall1, cb));
}


/* Unmarshall route_info_changed6 */
void
XrlRibClientV0p1Client::unmarshall1(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB1		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibClientV0p1Client::send_route_info_invalid4(
	const char*	the_tgt, 
	const IPv4&	addr, 
	const uint32_t&	prefix_len, 
	const CB2&	cb
)
{
    Xrl x(the_tgt, "rib_client/0.1/route_info_invalid4");
    x.args().add("addr", addr);
    x.args().add("prefix_len", prefix_len);
    return _sender->send(x, callback(this, &XrlRibClientV0p1Client::unmarshall2, cb));
}


/* Unmarshall route_info_invalid4 */
void
XrlRibClientV0p1Client::unmarshall2(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB2		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibClientV0p1Client::send_route_info_invalid6(
	const char*	the_tgt, 
	const IPv6&	addr, 
	const uint32_t&	prefix_len, 
	const CB3&	cb
)
{
    Xrl x(the_tgt, "rib_client/0.1/route_info_invalid6");
    x.args().add("addr", addr);
    x.args().add("prefix_len", prefix_len);
    return _sender->send(x, callback(this, &XrlRibClientV0p1Client::unmarshall3, cb));
}


/* Unmarshall route_info_invalid6 */
void
XrlRibClientV0p1Client::unmarshall3(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB3		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}
