/*
 * Copyright (c) 2001-2003 International Computer Science Institute
 * See LICENSE file for licensing, conditions, and warranties on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by 'clnt-gen'.
 *
 * $XORP$
 */

#ifndef __XRL_INTERFACES_XORP_IF_MIB_XIF_HH__
#define __XRL_INTERFACES_XORP_IF_MIB_XIF_HH__

#undef XORP_LIBRARY_NAME
#define XORP_LIBRARY_NAME "XifXorpIfMib"

#include "libxorp/xlog.h"
#include "libxorp/callback.hh"

#include "libxipc/xrl.hh"
#include "libxipc/xrl_error.hh"
#include "libxipc/xrl_sender.hh"


class XrlXorpIfMibV0p1Client {
public:
    XrlXorpIfMibV0p1Client(XrlSender* s) : _sender(s) {}
    virtual ~XrlXorpIfMibV0p1Client() {}

    typedef XorpCallback2<void, const XrlError&, const uint32_t*>::RefPtr CB0;
    /**
     *  Send Xrl intended to:
     *  
     *  Load a MIB module
     *  
     *  @param tgt_name Xrl Target name
     *  
     *  @param mod_name the mib module file name (without extension)
     *  
     *  @param abs_path absolute path to the module file
     */
    bool send_load_mib(
	const char*	target_name, 
	const string&	mod_name, 
	const string&	abs_path, 
	const CB0&	cb
    );

    typedef XorpCallback2<void, const XrlError&, const bool*>::RefPtr CB1;
    /**
     *  Send Xrl intended to:
     *  
     *  Unload a MIB module
     *  
     *  @param tgt_name Xrl Target name
     */
    bool send_unload_mib(
	const char*	target_name, 
	const uint32_t&	mib_index, 
	const CB1&	cb
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

};

#endif /* __XRL_INTERFACES_XORP_IF_MIB_XIF_HH__ */
