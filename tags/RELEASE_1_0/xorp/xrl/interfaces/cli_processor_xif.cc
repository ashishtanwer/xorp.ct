/*
 * Copyright (c) 2001-2004 International Computer Science Institute
 * See LICENSE file for licensing, conditions, and warranties on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by 'clnt-gen'.
 */

#ident "$XORP: xorp/xrl/interfaces/cli_processor_xif.cc,v 1.9 2004/05/24 17:35:33 hodson Exp $"

#include "cli_processor_xif.hh"

bool
XrlCliProcessorV0p1Client::send_process_command(
	const char*	the_tgt,
	const string&	processor_name,
	const string&	cli_term_name,
	const uint32_t&	cli_session_id,
	const string&	command_name,
	const string&	command_args,
	const ProcessCommandCB&	cb
)
{
    Xrl x(the_tgt, "cli_processor/0.1/process_command");
    x.args().add("processor_name", processor_name);
    x.args().add("cli_term_name", cli_term_name);
    x.args().add("cli_session_id", cli_session_id);
    x.args().add("command_name", command_name);
    x.args().add("command_args", command_args);
    return _sender->send(x, callback(this, &XrlCliProcessorV0p1Client::unmarshall_process_command, cb));
}


/* Unmarshall process_command */
void
XrlCliProcessorV0p1Client::unmarshall_process_command(
	const XrlError&	e,
	XrlArgs*	a,
	ProcessCommandCB		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e, 0, 0, 0, 0);
	return;
    } else if (a && a->size() != 4) {
	XLOG_ERROR("Wrong number of arguments (%u != %u)", (uint32_t)a->size(), 4);
	cb->dispatch(XrlError::BAD_ARGS(), 0, 0, 0, 0);
	return;
    }
    string ret_processor_name;
    string ret_cli_term_name;
    uint32_t ret_cli_session_id;
    string ret_command_output;
    try {
	a->get("ret_processor_name", ret_processor_name);
	a->get("ret_cli_term_name", ret_cli_term_name);
	a->get("ret_cli_session_id", ret_cli_session_id);
	a->get("ret_command_output", ret_command_output);
    } catch (const XrlArgs::XrlAtomNotFound&) {
	XLOG_ERROR("Atom not found");
	cb->dispatch(XrlError::BAD_ARGS(), 0, 0, 0, 0);
	return;
    }
    cb->dispatch(e, &ret_processor_name, &ret_cli_term_name, &ret_cli_session_id, &ret_command_output);
}
