/*
 * Copyright (c) 2001-2003 International Computer Science Institute
 * See LICENSE file for licensing, conditions, and warranties on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by 'clnt-gen'.
 */

#ident "$XORP: xorp/xrl/interfaces/fea_rawpkt_xif.cc,v 1.5 2003/03/10 23:01:28 hodson Exp $"

#include "fea_rawpkt_xif.hh"

bool
XrlRawPacketV0p1Client::send_send4(
	const char*	the_tgt, 
	const IPv4&	src_address, 
	const IPv4&	dst_address, 
	const string&	vif_name, 
	const uint32_t&	proto, 
	const uint32_t&	ttl, 
	const uint32_t&	tos, 
	const vector<uint8_t>&	options, 
	const vector<uint8_t>&	payload, 
	const CB0&	cb
)
{
    Xrl x(the_tgt, "raw_packet/0.1/send4");
    x.args().add("src_address", src_address);
    x.args().add("dst_address", dst_address);
    x.args().add("vif_name", vif_name);
    x.args().add("proto", proto);
    x.args().add("ttl", ttl);
    x.args().add("tos", tos);
    x.args().add("options", options);
    x.args().add("payload", payload);
    return _sender->send(x, callback(this, &XrlRawPacketV0p1Client::unmarshall0, cb));
}


/* Unmarshall send4 */
void
XrlRawPacketV0p1Client::unmarshall0(
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
XrlRawPacketV0p1Client::send_send_raw4(
	const char*	the_tgt, 
	const string&	vif_name, 
	const vector<uint8_t>&	packet, 
	const CB1&	cb
)
{
    Xrl x(the_tgt, "raw_packet/0.1/send_raw4");
    x.args().add("vif_name", vif_name);
    x.args().add("packet", packet);
    return _sender->send(x, callback(this, &XrlRawPacketV0p1Client::unmarshall1, cb));
}


/* Unmarshall send_raw4 */
void
XrlRawPacketV0p1Client::unmarshall1(
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
XrlRawPacketV0p1Client::send_register_vif_receiver(
	const char*	the_tgt, 
	const string&	router_name, 
	const string&	if_name, 
	const string&	vif_name, 
	const uint32_t&	proto, 
	const CB2&	cb
)
{
    Xrl x(the_tgt, "raw_packet/0.1/register_vif_receiver");
    x.args().add("router_name", router_name);
    x.args().add("if_name", if_name);
    x.args().add("vif_name", vif_name);
    x.args().add("proto", proto);
    return _sender->send(x, callback(this, &XrlRawPacketV0p1Client::unmarshall2, cb));
}


/* Unmarshall register_vif_receiver */
void
XrlRawPacketV0p1Client::unmarshall2(
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
XrlRawPacketV0p1Client::send_unregister_vif_receiver(
	const char*	the_tgt, 
	const string&	router_name, 
	const string&	if_name, 
	const string&	vif_name, 
	const uint32_t&	proto, 
	const CB3&	cb
)
{
    Xrl x(the_tgt, "raw_packet/0.1/unregister_vif_receiver");
    x.args().add("router_name", router_name);
    x.args().add("if_name", if_name);
    x.args().add("vif_name", vif_name);
    x.args().add("proto", proto);
    return _sender->send(x, callback(this, &XrlRawPacketV0p1Client::unmarshall3, cb));
}


/* Unmarshall unregister_vif_receiver */
void
XrlRawPacketV0p1Client::unmarshall3(
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
