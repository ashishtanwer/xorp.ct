/*
 * Copyright (c) 2001-2003 International Computer Science Institute
 * See LICENSE file for licensing, conditions, and warranties on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by 'clnt-gen'.
 *
 * $XORP: xorp/xrl/interfaces/datain_xif.hh,v 1.5 2003/03/10 23:01:27 hodson Exp $
 */

#ifndef __XRL_INTERFACES_DATAIN_XIF_HH__
#define __XRL_INTERFACES_DATAIN_XIF_HH__

#undef XORP_LIBRARY_NAME
#define XORP_LIBRARY_NAME "XifDatain"

#include "libxorp/xlog.h"
#include "libxorp/callback.hh"

#include "libxipc/xrl.hh"
#include "libxipc/xrl_error.hh"
#include "libxipc/xrl_sender.hh"


class XrlDatainV0p1Client {
public:
    XrlDatainV0p1Client(XrlSender* s) : _sender(s) {}
    virtual ~XrlDatainV0p1Client() {}

    typedef XorpCallback1<void, const XrlError&>::RefPtr CB0;

    bool send_receive(
	const char*	target_name, 
	const string&	peer, 
	const bool&	status, 
	const uint32_t&	secs, 
	const uint32_t&	micro, 
	const vector<uint8_t>&	data, 
	const CB0&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr CB1;

    bool send_error(
	const char*	target_name, 
	const string&	peer, 
	const string&	reason, 
	const CB1&	cb
    );

    typedef XorpCallback1<void, const XrlError&>::RefPtr CB2;

    bool send_closed(
	const char*	target_name, 
	const string&	peer, 
	const CB2&	cb
    );

protected:
    XrlSender* _sender;

private:
    void unmarshall0(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB0		cb
    );

    void unmarshall1(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB1		cb
    );

    void unmarshall2(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB2		cb
    );

};

#endif /* __XRL_INTERFACES_DATAIN_XIF_HH__ */
