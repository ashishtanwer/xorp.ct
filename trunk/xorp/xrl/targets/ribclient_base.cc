/*
 * Copyright (c) 2001-2003 International Computer Science Institute
 * See LICENSE file for licensing, conditions, and warranties on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by 'tgt-gen'.
 */

#ident "$XORP: xorp/xrl/targets/ribclient_base.cc,v 1.4 2003/03/29 19:03:14 pavlin Exp $"


#include "ribclient_base.hh"

const XrlCmdError
XrlRibclientTargetBase::handle_rib_client_0_1_route_info_changed4(const Xrl& xrl, XrlArgs* /* args */)
{
    if (xrl.const_args().size() != 6) { 
	XLOG_ERROR("Wrong number of arguments (%u != 6) handling rib_client/0.1/route_info_changed4", (uint32_t)xrl.const_args().size());
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = rib_client_0_1_route_info_changed4(
	    xrl.const_args().get_ipv4("addr"), 
	    xrl.const_args().get_uint32("prefix_len"), 
	    xrl.const_args().get_ipv4("nexthop"), 
	    xrl.const_args().get_uint32("metric"), 
	    xrl.const_args().get_uint32("admin_distance"), 
	    xrl.const_args().get_string("protocol_origin"));
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for rib_client/0.1/route_info_changed4 failed: %s \"%s\"",
            		 e.str().c_str(), e.note().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlRibclientTargetBase::handle_rib_client_0_1_route_info_changed6(const Xrl& xrl, XrlArgs* /* args */)
{
    if (xrl.const_args().size() != 6) { 
	XLOG_ERROR("Wrong number of arguments (%u != 6) handling rib_client/0.1/route_info_changed6", (uint32_t)xrl.const_args().size());
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = rib_client_0_1_route_info_changed6(
	    xrl.const_args().get_ipv6("addr"), 
	    xrl.const_args().get_uint32("prefix_len"), 
	    xrl.const_args().get_ipv6("nexthop"), 
	    xrl.const_args().get_uint32("metric"), 
	    xrl.const_args().get_uint32("admin_distance"), 
	    xrl.const_args().get_string("protocol_origin"));
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for rib_client/0.1/route_info_changed6 failed: %s \"%s\"",
            		 e.str().c_str(), e.note().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlRibclientTargetBase::handle_rib_client_0_1_route_info_invalid4(const Xrl& xrl, XrlArgs* /* args */)
{
    if (xrl.const_args().size() != 2) { 
	XLOG_ERROR("Wrong number of arguments (%u != 2) handling rib_client/0.1/route_info_invalid4", (uint32_t)xrl.const_args().size());
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = rib_client_0_1_route_info_invalid4(
	    xrl.const_args().get_ipv4("addr"), 
	    xrl.const_args().get_uint32("prefix_len"));
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for rib_client/0.1/route_info_invalid4 failed: %s \"%s\"",
            		 e.str().c_str(), e.note().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlRibclientTargetBase::handle_rib_client_0_1_route_info_invalid6(const Xrl& xrl, XrlArgs* /* args */)
{
    if (xrl.const_args().size() != 2) { 
	XLOG_ERROR("Wrong number of arguments (%u != 2) handling rib_client/0.1/route_info_invalid6", (uint32_t)xrl.const_args().size());
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = rib_client_0_1_route_info_invalid6(
	    xrl.const_args().get_ipv6("addr"), 
	    xrl.const_args().get_uint32("prefix_len"));
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for rib_client/0.1/route_info_invalid6 failed: %s \"%s\"",
            		 e.str().c_str(), e.note().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }
    return XrlCmdError::OKAY();
}

