// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2001,2002 International Computer Science Institute
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

#ident "$XORP$"

#define DEBUG_LOGGING
#define DEBUG_PRINT_FUNCTION_NAME

#include "libxorp_module.h"
#include "config.h"
#include "test_main.hh"
#include "exceptions.hh"

int
test1(TestInfo& info)
{
    if(info.verbose())
	info.out() << "verbose on\n";

    return TestMain::SUCCESS;
}

int
test2(TestInfo& info)
{
    if(info.verbose())
	info.out() << "verbose on level = " <<  info.verbose_level() << endl;

    return TestMain::SUCCESS;
}

int
test3(TestInfo& info, bool fail)
{
    if(info.verbose())
	info.out() << info.test_name() << " Test will " <<
	    (fail ? "fail" : "succeed") << endl;

    if(fail)
	return TestMain::FAILURE;

    return TestMain::SUCCESS;
}

int
test4(TestInfo& info, const char *mess)
{
    if(info.verbose())
	info.out() << "verbose on level = " << info.verbose_level() << 
	    " message = " <<  mess << endl;

    return TestMain::SUCCESS;
}

int
test5(TestInfo& info, bool exception)
{
    if(info.verbose())
	info.out() << info.test_name() << " Test will " <<
	    (exception ? "throw exception" : "succeed") << endl;

    if(exception)
	xorp_throw(InvalidString, "Hello");

    return TestMain::SUCCESS;
}

int
test6(TestInfo& info, bool exception)
{
    if(info.verbose())
	info.out() << info.test_name() << " Test will " <<
	    (exception ? "throw exception" : "succeed") << endl;

    if(exception)
	throw("Unexpected exception");

    return TestMain::SUCCESS;
}

int
main(int argc, char **argv)
{
    XorpUnexpectedHandler x(xorp_unexpected_handler);

    TestMain t(argc, argv);

    string test =
	t.get_optional_args("-t", "--test", "run only the specified test");
    bool fail =
	t.get_optional_flag("-f", "--fail", "fail test3");
    string mess = 
	t.get_optional_args("-m", "--message", "pass message to test4");
    bool exception = 
	t.get_optional_flag("-e", "--exception",
			    "throw xorp exception test5");
    bool unexpected = 
	t.get_optional_flag("-u", "--unexpected",
			    "throw unexpected exception test6");

    t.complete_args_parsing();

    struct test {
	string test_name;
	XorpCallback1<int, TestInfo&>::RefPtr cb;
    } tests[] = {
	{"test1", callback(test1)},
	{"test2", callback(test2)},
	{"test3", callback(test3, fail)},
	{"test4", callback(test4, mess.c_str())},
	{"test5", callback(test5, exception)},
	{"test6", callback(test6, unexpected)},
    };

    try {
	if("" == test) {
	    for(unsigned int i = 0; i < sizeof(tests) / sizeof(struct test); 
		i++)
		t.run(tests[i].test_name, tests[i].cb);
	} else {
	    for(unsigned int i = 0; i < sizeof(tests) / sizeof(struct test); 
		i++)
		if(test == tests[i].test_name) {
		    t.run(tests[i].test_name, tests[i].cb);
		    return t.exit();
		}
	    t.failed("No test with name " + test + " found\n");
	}
    } catch(...) {
	xorp_catch_standard_exceptions();
    }

    return t.exit();
}
