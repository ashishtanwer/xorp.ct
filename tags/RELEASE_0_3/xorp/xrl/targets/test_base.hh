/*
 * Copyright (c) 2001-2003 International Computer Science Institute
 * See LICENSE file for licensing, conditions, and warranties on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by 'tgt-gen'.
 *
 * $XORP: xorp/xrl/targets/test_base.hh,v 1.9 2003/05/29 21:17:23 mjh Exp $
 */


#ifndef __XRL_INTERFACES_TEST_BASE_HH__
#define __XRL_INTERFACES_TEST_BASE_HH__

#undef XORP_LIBRARY_NAME
#define XORP_LIBRARY_NAME "XrlTestTarget"

#include "libxorp/xlog.h"
#include "libxipc/xrl_cmd_map.hh"

class XrlTestTargetBase {
protected:
    XrlCmdMap* _cmds;

public:
    XrlTestTargetBase(XrlCmdMap* cmds) : _cmds(cmds) { add_handlers(); }
    virtual ~XrlTestTargetBase() { remove_handlers(); }

    inline const string& name() const { return _cmds->name(); }
    inline const char* version() const { return "test/0.0"; }

protected:

    /**
     *  Pure-virtual function that needs to be implemented to:
     *  
     *  Get name of Xrl Target
     */
    virtual XrlCmdError common_0_1_get_target_name(
	// Output values, 
	string&	name) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *  
     *  Get version string from Xrl Target
     */
    virtual XrlCmdError common_0_1_get_version(
	// Output values, 
	string&	version) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *  
     *  Get status of Xrl Target
     */
    virtual XrlCmdError common_0_1_get_status(
	// Output values, 
	uint32_t&	status, 
	string&	reason) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *  
     *  Request clean shutdown of Xrl Target
     */
    virtual XrlCmdError common_0_1_shutdown() = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *  
     *  Print hello world
     */
    virtual XrlCmdError test_1_0_print_hello_world() = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *  
     *  Print "hello world" and a user supplied greeting.
     *  
     *  @param msg greeting to be printed.
     */
    virtual XrlCmdError test_1_0_print_hello_world_and_message(
	// Input values, 
	const string&	msg) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *  
     *  Count number of greetings available
     */
    virtual XrlCmdError test_1_0_get_greeting_count(
	// Output values, 
	int32_t&	num_msgs) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *  
     *  Get greeting
     *  
     *  @param greeting_num index of greeting
     *  
     *  @param greeting text of greeting.
     */
    virtual XrlCmdError test_1_0_get_greeting(
	// Input values, 
	const int32_t&	greeting_num, 
	// Output values, 
	string&	greeting) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *  
     *  Something that always fails.
     */
    virtual XrlCmdError test_1_0_shoot_foot() = 0;

private:
    const XrlCmdError handle_common_0_1_get_target_name(const Xrl& x, XrlArgs* a);

    const XrlCmdError handle_common_0_1_get_version(const Xrl& x, XrlArgs* a);

    const XrlCmdError handle_common_0_1_get_status(const Xrl& x, XrlArgs* a);

    const XrlCmdError handle_common_0_1_shutdown(const Xrl& x, XrlArgs* a);

    const XrlCmdError handle_test_1_0_print_hello_world(const Xrl& x, XrlArgs* a);

    const XrlCmdError handle_test_1_0_print_hello_world_and_message(const Xrl& x, XrlArgs* a);

    const XrlCmdError handle_test_1_0_get_greeting_count(const Xrl& x, XrlArgs* a);

    const XrlCmdError handle_test_1_0_get_greeting(const Xrl& x, XrlArgs* a);

    const XrlCmdError handle_test_1_0_shoot_foot(const Xrl& x, XrlArgs* a);

    void add_handlers()
    {
	if (_cmds->add_handler("common/0.1/get_target_name", 
	    callback(this, &XrlTestTargetBase::handle_common_0_1_get_target_name)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://test/common/0.1/get_target_name");
	}
	if (_cmds->add_handler("common/0.1/get_version", 
	    callback(this, &XrlTestTargetBase::handle_common_0_1_get_version)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://test/common/0.1/get_version");
	}
	if (_cmds->add_handler("common/0.1/get_status", 
	    callback(this, &XrlTestTargetBase::handle_common_0_1_get_status)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://test/common/0.1/get_status");
	}
	if (_cmds->add_handler("common/0.1/shutdown", 
	    callback(this, &XrlTestTargetBase::handle_common_0_1_shutdown)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://test/common/0.1/shutdown");
	}
	if (_cmds->add_handler("test/1.0/print_hello_world", 
	    callback(this, &XrlTestTargetBase::handle_test_1_0_print_hello_world)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://test/test/1.0/print_hello_world");
	}
	if (_cmds->add_handler("test/1.0/print_hello_world_and_message", 
	    callback(this, &XrlTestTargetBase::handle_test_1_0_print_hello_world_and_message)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://test/test/1.0/print_hello_world_and_message");
	}
	if (_cmds->add_handler("test/1.0/get_greeting_count", 
	    callback(this, &XrlTestTargetBase::handle_test_1_0_get_greeting_count)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://test/test/1.0/get_greeting_count");
	}
	if (_cmds->add_handler("test/1.0/get_greeting", 
	    callback(this, &XrlTestTargetBase::handle_test_1_0_get_greeting)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://test/test/1.0/get_greeting");
	}
	if (_cmds->add_handler("test/1.0/shoot_foot", 
	    callback(this, &XrlTestTargetBase::handle_test_1_0_shoot_foot)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://test/test/1.0/shoot_foot");
	}
	_cmds->finalize();
    }

    void remove_handlers()
    {
	_cmds->remove_handler("common/0.1/get_target_name");
	_cmds->remove_handler("common/0.1/get_version");
	_cmds->remove_handler("common/0.1/get_status");
	_cmds->remove_handler("common/0.1/shutdown");
	_cmds->remove_handler("test/1.0/print_hello_world");
	_cmds->remove_handler("test/1.0/print_hello_world_and_message");
	_cmds->remove_handler("test/1.0/get_greeting_count");
	_cmds->remove_handler("test/1.0/get_greeting");
	_cmds->remove_handler("test/1.0/shoot_foot");
    }
};

#endif /* __XRL_INTERFACES_TEST_BASE_HH__ */
