// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-
// vim:set sts=4 ts=8:

// Copyright (c) 2001-2009 XORP, Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License, Version
// 2.1, June 1999 as published by the Free Software Foundation.
// Redistribution and/or modification of this program under the terms of
// any other version of the GNU Lesser General Public License is not
// permitted.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more details,
// see the GNU Lesser General Public License, Version 2.1, a copy of
// which can be found in the XORP LICENSE.lgpl file.
// 
// XORP, Inc, 2953 Bunker Hill Lane, Suite 204, Santa Clara, CA 95054, USA;
// http://xorp.net

//#define DEBUG_LOGGING



#include "libxorp/xorp.h"

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include <boost/noncopyable.hpp>

#include "libxorp/debug.h"
#include "libxipc/xrl_module.h"
#include "libxorp/xlog.h"
#include "libxorp/buffered_asyncio.hh"

#include "libcomm/comm_api.h"

#include "sockutil.hh"

#include "xrl.hh"
#include "xrl_error.hh"
#include "xrl_pf_stcp.hh"
#include "xrl_pf_stcp_ph.hh"
#include "xrl_dispatcher.hh"

const char* XrlPFSTCPSender::_protocol   = "stcp";
const char* XrlPFSTCPListener::_protocol = "stcp";

// The maximum number of bytes worth of XRL buffered before send()
// returns false.  Resource preservation.
static const size_t 	MAX_ACTIVE_BYTES 	    = 100000;

// The maximum number of XRLs buffered at the sender.
static const size_t 	MAX_ACTIVE_REQUESTS  	    = 100;

// The maximum number of XRLs the receiver will dispatch per read event, ie
// per read() system call.
static const uint32_t   MAX_XRLS_DISPATCHED	    = 2;

// The maximum number of buffers the AsyncFileWriters should coalesce.
static const uint32_t   MAX_WRITES		    = 16;

#define xassert(x) // An expensive - assert(x)


// ----------------------------------------------------------------------------
// STCPRequestHandler - created by Listener to manage requests over a
// connection.  These are allocated on by XrlPFSTCPListener and delete
// themselves using a timeout timer when they have been quiescent for a while

class STCPRequestHandler {
protected:
    static const TimeVal	DEFAULT_KEEPALIVE_TIMEOUT;
public:
    STCPRequestHandler(XrlPFSTCPListener& parent, XorpFd sock) :
	_parent(parent), _sock(sock),
	_reader(parent.eventloop(), sock, 4 * 65536,
		callback(this, &STCPRequestHandler::read_event)),
	_writer(parent.eventloop(), sock, MAX_WRITES),
	_responses_size(0),
	_keepalive_timeout(DEFAULT_KEEPALIVE_TIMEOUT)
    {
	EventLoop& e = _parent.eventloop();

	// Set the STCP request timeout from environment variable if it is set.
	char* value = getenv("XORP_LISTENER_KEEPALIVE_TIMEOUT");
	if (value != NULL) {
	    char* ep = NULL;
	    uint32_t timeout_s = 0;
	    timeout_s = strtoul(value, &ep, 10);
	    if ( !(*value != '\0' && *ep == '\0') &&
		  (timeout_s <= 0 || timeout_s > 300)) {
		XLOG_ERROR("Invalid \"XORP_LISTENER_KEEPALIVE_TIMEOUT\": %s",
			   value);
	    } else {
		_keepalive_timeout = TimeVal(timeout_s, 0);
	    }
	}

	if (! _keepalive_timeout.is_zero()) {
	    _life_timer = e.new_oneoff_after(_keepalive_timeout,
					     callback(this,
						      &STCPRequestHandler::die,
						      "life timer expired",
						      true));
	}

	_reader.start();
	debug_msg("STCPRequestHandler (%p) fd = %s\n",
		  this, sock.str().c_str());
    }

    ~STCPRequestHandler()
    {
	_parent.remove_request_handler(this);
	_reader.stop();
	_writer.stop();
	debug_msg("STCPRequestHandler (%p) fd = %s\n",
		  this, _sock.str().c_str());
	comm_close(_sock);
	_sock.clear();
    }

    void dispatch_request(uint32_t seqno, bool batch, const uint8_t* buffer,
			  size_t bytes);
    void ack_helo(uint32_t seqno);

    void read_event(BufferedAsyncReader* 	reader,
		    BufferedAsyncReader::Event 	e,
		    uint8_t* 			buffer,
		    size_t 			buffer_bytes);

    void update_writer(AsyncFileWriter::Event,
		       const uint8_t*,
		       size_t,
		       size_t);

    // Death related
    void postpone_death();
    void die(const char* reason, bool verbose = true);

    bool response_pending() const;

private:
    XrlError do_dispatch(const uint8_t* packed_xrl, size_t packed_xrl_bytes,
                         XrlArgs& response);

    XrlPFSTCPListener& _parent;
    XorpFd _sock;

    // Reader associated with buffer
    BufferedAsyncReader _reader;

    // Writer associated current response (head of responses).
    AsyncFileWriter _writer;

    typedef vector<uint8_t> ReplyPacket;
    list<ReplyPacket> 	_responses; 	// head is currently being written
    uint32_t		_responses_size;

    // If the STCP keepalive timeout is non-zero, then STCPRequestHandlers
    // will delete themselves if quiescent for timeout period. Otherwise,
    // we rely upon socket layer notification of a TCP session close in
    // AsyncFileReader/Writer to close STCP sessions.
    TimeVal		_keepalive_timeout;
    XorpTimer		_life_timer;

    void parse_header(const uint8_t* buffer, size_t buffer_bytes);
    void parse_payload();
};

// Default life timer for STCP session is 3 minutes.
const TimeVal STCPRequestHandler::DEFAULT_KEEPALIVE_TIMEOUT = TimeVal(180, 0);

void
STCPRequestHandler::read_event(BufferedAsyncReader*		/* source */,
			       BufferedAsyncReader::Event 	ev,
			       uint8_t*				buffer,
			       size_t   			buffer_bytes)
{
    if (ev == BufferedAsyncReader::OS_ERROR) {
	XLOG_ERROR("Read failed (error = %d)\n", _reader.error());
	die("read error");
	return;
    }

    if (ev == BufferedAsyncReader::END_OF_FILE) {
	die("end of file", false);
	return;
    }

    for (u_int iters = 0; iters < MAX_XRLS_DISPATCHED; iters++) {
	if (buffer_bytes < STCPPacketHeader::header_size()) {
	    // Not enough data to even inspect the header
	    size_t new_trigger_bytes = STCPPacketHeader::header_size() - buffer_bytes;
	    _reader.set_trigger_bytes(new_trigger_bytes);
	    return;
	}

	const STCPPacketHeader sph(buffer);

	if (!sph.is_valid()) {
	    die("bad header");
	    return;
	}

	if (sph.type() == STCP_PT_HELO) {
	    debug_msg("Got helo\n");
	    ack_helo(sph.seqno());
	    _reader.dispose(sph.frame_bytes());
	    _reader.set_trigger_bytes(STCPPacketHeader::header_size());
	    return;
	} else if (sph.type() != STCP_PT_REQUEST) {
	    die("Bad packet type");
	    return;
	}

	if (buffer_bytes >= sph.frame_bytes()) {
	    uint8_t* xrl_data = buffer;
	    xrl_data += STCPPacketHeader::header_size() + sph.error_note_bytes();
	    size_t   xrl_data_bytes = sph.payload_bytes();
	    dispatch_request(sph.seqno(), sph.batch(),
			     xrl_data, xrl_data_bytes);
	    _reader.dispose(sph.frame_bytes());
	    buffer += sph.frame_bytes();
	    buffer_bytes -= sph.frame_bytes();
	} else {
	    size_t frame_bytes = sph.frame_bytes();
	    if (frame_bytes > _reader.reserve_bytes()) {
		_reader.set_reserve_bytes(frame_bytes);
	    }
	    _reader.set_trigger_bytes(frame_bytes);
	    return;
	}
    }
    _reader.set_trigger_bytes(STCPPacketHeader::header_size());
}

XrlError
STCPRequestHandler::do_dispatch(const uint8_t* packed_xrl,
			        size_t packed_xrl_bytes,
			        XrlArgs& response)
{
    static XrlError e(XrlError::INTERNAL_ERROR().error_code(), "corrupt xrl");

    const XrlDispatcher* d = _parent.dispatcher();
    assert(d != 0);

    string command;
    size_t cmdsz = Xrl::unpack_command(command, packed_xrl, packed_xrl_bytes);
    if (!cmdsz)
	return e;

    XrlDispatcher::XI* xi = d->lookup_xrl(command);
    if (!xi)
	return e;

    Xrl& xrl = xi->_xrl;

    try {
	if (xi->_new) {
	    if (xrl.unpack(packed_xrl, packed_xrl_bytes) != packed_xrl_bytes)
		return e;

	    xi->_new = false;
	} else {
	    packed_xrl       += cmdsz;
	    packed_xrl_bytes -= cmdsz;

	    if (xrl.fill(packed_xrl, packed_xrl_bytes) != packed_xrl_bytes)
		return e;
	}
    } catch (...) {
	return e;
    }

    return d->dispatch_xrl_fast(*xi, response);
}

void
STCPRequestHandler::dispatch_request(uint32_t 		seqno,
				     bool		batch,
				     const uint8_t* 	packed_xrl,
				     size_t 		packed_xrl_bytes)
{
    XrlArgs response;
    XrlError e;

    e = do_dispatch(packed_xrl, packed_xrl_bytes, response);

    size_t xrl_response_bytes = response.packed_bytes();
    size_t note_bytes = e.note().size();

    _responses.push_back(ReplyPacket(STCPPacketHeader::header_size()
			 + note_bytes + xrl_response_bytes));

    _responses_size++;
    ReplyPacket& r = _responses.back();

    STCPPacketHeader sph(&r[0]);
    sph.initialize(seqno, STCP_PT_RESPONSE, e, xrl_response_bytes);

    if (note_bytes != 0) {
	memcpy(&r[0] + STCPPacketHeader::header_size(),
	       e.note().c_str(), note_bytes);
    }

    if (xrl_response_bytes != 0) {
	response.pack(&r[0] + STCPPacketHeader::header_size() + note_bytes,
		      xrl_response_bytes);
    }

    _writer.add_buffer(&r[0], r.size(),
		       callback(this, &STCPRequestHandler::update_writer));

    debug_msg("about to start_writer (%d)\n", _responses.size() != 0);
    if (!batch && _writer.running() == false) {
	_writer.start();
    }
}

void
STCPRequestHandler::ack_helo(uint32_t seqno)
{
    _responses.push_back(ReplyPacket(STCPPacketHeader::header_size()));
    _responses_size++;
    ReplyPacket& r = _responses.back();

    STCPPacketHeader sph(&r[0]);
    sph.initialize(seqno, STCP_PT_HELO_ACK, XrlError::OKAY(), 0);
    _writer.add_buffer(&r[0], r.size(),
		       callback(this, &STCPRequestHandler::update_writer));

    xassert(_writer.buffers_remaining() == _responses.size());

    if (_writer.running() == false) {
	_writer.start();
    }
    assert(_responses.empty() || _writer.running());
}

void
STCPRequestHandler::update_writer(AsyncFileWriter::Event ev,
				  const uint8_t*	 /* buffer */,
				  size_t		 /* buffer_bytes */,
				  size_t		 bytes_done)
{
    postpone_death();

    if (ev == AsyncFileWriter::FLUSHING)
	return;	// code pre-dates FLUSHING event

    debug_msg("Writer offset %u\n", XORP_UINT_CAST(bytes_done));

    if (ev == AsyncFileWriter::OS_ERROR && _writer.error() != EWOULDBLOCK) {
	die("write failed");
	return;
    }

    debug_msg("responses size %u ready %u\n",
	      XORP_UINT_CAST(_responses.size()),
	      XORP_UINT_CAST(_reader.available_bytes()));

    if (_responses.front().size() == bytes_done) {
	xassert(_responses.size() <= MAX_ACTIVE_REQUESTS);
	debug_msg("Packet completed -> %u bytes written.\n",
		  XORP_UINT_CAST(_responses.front().size()));
	// erase old head
	_responses.pop_front();
	_responses_size--;
	xassert(_responses.empty() || _responses.size() == _responses_size);
	// restart writer if necessary
	if (_writer.running() == false && _responses.empty() == false)
	    _writer.start();
	return;
    }
}

// Methods and constants relating to death
void
STCPRequestHandler::postpone_death()
{
    if (! _keepalive_timeout.is_zero())
	_life_timer.schedule_after(_keepalive_timeout);
}

void
STCPRequestHandler::die(const char *reason, bool verbose)
{
    debug_msg("%s", reason);
    if (verbose)
	XLOG_ERROR("STCPRequestHandler died: %s", reason);
    delete this;
}

bool
STCPRequestHandler::response_pending() const
{
    return (_responses.empty() == false) || (_writer.running() == true);
}


// ----------------------------------------------------------------------------
// Simple TCP Listener - creates TCPRequestHandlers for each incoming
// connection.

XrlPFSTCPListener::XrlPFSTCPListener(EventLoop&	    e,
				     XrlDispatcher* x,
				     uint16_t	    port)
    throw (XrlPFConstructorError)
    : XrlPFListener(e, x), _address_slash_port()
{
    in_addr myaddr = get_preferred_ipv4_addr();

    _sock = comm_bind_tcp4(&myaddr, port, COMM_SOCK_NONBLOCKING);
    if (!_sock.is_valid()) {
	xorp_throw(XrlPFConstructorError,
		   comm_get_last_error_str());
    }
    if (comm_listen(_sock, COMM_LISTEN_DEFAULT_BACKLOG) != XORP_OK) {
	xorp_throw(XrlPFConstructorError,
		   comm_get_last_error_str());
    }

    string addr;
    if (get_local_socket_details(_sock, addr, port) == false) {
	int err = comm_get_last_error();
        comm_close(_sock);
	_sock.clear();
        xorp_throw(XrlPFConstructorError, comm_get_error_str(err));
    }

    _address_slash_port = address_slash_port(addr, port);
    _eventloop.add_ioevent_cb(_sock, IOT_ACCEPT,
			     callback(this, &XrlPFSTCPListener::connect_hook));
}

XrlPFSTCPListener::XrlPFSTCPListener(EventLoop* e, XrlDispatcher* x)
    : XrlPFListener(*e, x)
{
}

XrlPFSTCPListener::~XrlPFSTCPListener()
{
    while (_request_handlers.empty() == false) {
	delete _request_handlers.front();
	// nb destructor for STCPRequestHandler triggers removal of node
	// from list
    }
    _eventloop.remove_ioevent_cb(_sock, IOT_ACCEPT);
    comm_close(_sock);
    _sock.clear();
}

void
XrlPFSTCPListener::connect_hook(XorpFd fd, IoEventType /* type */)
{
    XorpFd cfd = comm_sock_accept(fd);
    if (!cfd.is_valid()) {
	debug_msg("accept failed: %s\n",
		  comm_get_last_error_str());
	return;
    }
    comm_sock_set_blocking(cfd, COMM_SOCK_NONBLOCKING);
    add_request_handler(new STCPRequestHandler(*this, cfd));
}

void
XrlPFSTCPListener::add_request_handler(STCPRequestHandler* h)
{
    // assert handler is not already in list
    assert(find(_request_handlers.begin(), _request_handlers.end(), h)
	   == _request_handlers.end());
    _request_handlers.push_back(h);
}

void
XrlPFSTCPListener::remove_request_handler(const STCPRequestHandler* rh)
{
    list<STCPRequestHandler*>::iterator i;
    i = find(_request_handlers.begin(), _request_handlers.end(), rh);
    assert(i != _request_handlers.end());
    _request_handlers.erase(i);
}

bool
XrlPFSTCPListener::response_pending() const
{
    list<STCPRequestHandler*>::const_iterator ci;

    for (ci = _request_handlers.begin(); ci != _request_handlers.end(); ++ci) {
	STCPRequestHandler* l = *ci;
	if (l->response_pending())
	    return true;
    }

    return false;
}


/**
 * @short Sender state for tracking Xrl's forwarded by TCP.
 *
 * At some point this class should be re-factored to hold rendered Xrl
 * in wire format rather than as an Xrl since the existing scheme
 * requires additional copy operations.
 */
class RequestState :
    public boost::noncopyable
{
public:
    typedef XrlPFSender::SendCallback Callback;

public:
    RequestState(XrlPFSTCPSender* p,
		 uint32_t	  sn,
		 bool		  batch,
		 const Xrl&	  x,
		 const Callback&  cb)
	: _p(p), _sn(sn), _b(_buffer), _cb(cb), _keepalive(false)
    {
	size_t header_bytes = STCPPacketHeader::header_size();
	size_t xrl_bytes = x.packed_bytes();
	size_t total = header_bytes + xrl_bytes;

	if (total > sizeof(_buffer))
	    _b = new uint8_t[total];

	_size = total;

	// Prepare header
	STCPPacketHeader sph(_b);
	sph.initialize(_sn, STCP_PT_REQUEST, XrlError::OKAY(), xrl_bytes);
	sph.set_batch(batch);

	// Pack XRL data
	x.pack(_b + header_bytes, xrl_bytes);

	debug_msg("RequestState (%p - seqno %u)\n", this, XORP_UINT_CAST(sn));
	debug_msg("RequestState Xrl = %s\n", x.str().c_str());
    }

    RequestState(XrlPFSTCPSender* p, uint32_t sn)
	: _p(p), _sn(sn), _b(_buffer), _keepalive(true)
    {
	size_t header_bytes = STCPPacketHeader::header_size();

	XLOG_ASSERT(sizeof(_buffer) >= header_bytes);

	_size = header_bytes;

	// Prepare header
	STCPPacketHeader sph(_b);
	sph.initialize(_sn, STCP_PT_HELO, XrlError::OKAY(), 0);
    }

    bool		has_seqno(uint32_t n) const { return _sn == n; }
    XrlPFSTCPSender*	parent() const		{ return _p; }
    uint32_t		seqno() const		{ return _sn; }
    uint8_t*		buffer() 		{ return _b; }
    Callback&		cb() 			{ return _cb; }
    uint32_t		size() const		{ return _size; }

    bool is_keepalive()
    {
	if (size() < STCPPacketHeader::header_size())
	    return false;
	const STCPPacketHeader sph(_b);
	STCPPacketType pt = sph.type();
	return pt == STCP_PT_HELO || pt == STCP_PT_HELO_ACK;
    }

    void set_batch(bool batch)
    {
	STCPPacketHeader sph(_b);
	sph.set_batch(batch);
    }

    ~RequestState()
    {
	//	debug_msg("~RequestState (%p - seqno %u)\n", this, seqno());
	if (_b != _buffer)
	    delete [] _b;
    }

private:
    XrlPFSTCPSender*	_p;				// parent
    uint32_t		_sn;				// sequence number
    uint8_t*		_b;
    uint8_t		_buffer[256];	// XXX important performance parameter
    uint32_t		_size;
    Callback		_cb;
    bool		_keepalive;
};


// ----------------------------------------------------------------------------
// XrlPFSenderList
//
// A class to track instances of XrlPFSTCPSender.  Whenever a sender
// is created, it gets add to the SenderList and when it is
// destructed, it gets removed.
//
// This class exists because there's a potential race.  An STCP sender
// may have a queue of Xrls to dispatch when it fails.  When the
// failure happens we walk the queue of Xrls and invoke the callbacks
// with a failure argument.  Unfortunately these callbacks may delete
// the Sender whilst we are process the list.
//
// To make life easier, we splice items on the callback list to a
// stack allocated list structure (see XrlPFSender::die()).  As we're
// invoking the callbacks we check that the instance number of the sender
// that died is on the list.  If it's not, then dispatching the callback is
// definitely unsafe.

static class XrlPFSTCPSenderList {
public:
    void
    add_instance(uint32_t uid)
    {
	_uids.push_back(uid);
    }
    void
    remove_instance(uint32_t uid)
    {
	vector<uint32_t>::iterator i = find(_uids.begin(), _uids.end(), uid);
	if (i != _uids.end()) {
	    _uids.erase(i);
	}
    }
    bool
    valid_instance(uint32_t uid) const
    {
	vector<uint32_t>::const_iterator i = find(_uids.begin(),
						  _uids.end(),
						  uid);
	return (i != _uids.end());
    }
protected:
    vector<uint32_t> _uids;
} sender_list;



// ----------------------------------------------------------------------------
// Xrl Simple TCP protocol family sender -> -> -> XRLPFSTCPSender

static uint32_t direct_calls = 0;
static uint32_t indirect_calls = 0;

const TimeVal XrlPFSTCPSender::DEFAULT_SENDER_KEEPALIVE_PERIOD = TimeVal(10, 0);

uint32_t XrlPFSTCPSender::_next_uid = 0;

XrlPFSTCPSender::XrlPFSTCPSender(EventLoop& e, const char* addr_slash_port,
				 TimeVal keepalive_time)
    throw (XrlPFConstructorError)
    : XrlPFSender(e, addr_slash_port),
      _uid(_next_uid++),
      _keepalive_time(keepalive_time),
      _batching(false)
{
    _sock = create_connected_tcp4_socket(addr_slash_port);
    construct();
}

XrlPFSTCPSender::XrlPFSTCPSender(EventLoop* e, const char* addr_slash_port,
				 TimeVal keepalive_time)
    : XrlPFSender(*e, addr_slash_port),
      _uid(_next_uid++), _writer(NULL),
      _keepalive_time(keepalive_time),
      _reader(NULL), _batching(false)
{
}

void
XrlPFSTCPSender::construct()
{
    debug_msg("stcp sender (%p) fd = %s\n", this, _sock.str().c_str());
    if (!_sock.is_valid()) {
	debug_msg("failed to connect to %s\n", address().c_str());
	xorp_throw(XrlPFConstructorError,
		   c_format("Could not connect to %s\n", address().c_str()));
    }

    if (comm_sock_set_blocking(_sock, 0) != XORP_OK) {
	debug_msg("failed to go non-blocking.\n");
	int err = comm_get_last_error();
	comm_close(_sock);
	_sock.clear();
	xorp_throw(XrlPFConstructorError,
		   c_format("Failed to set fd non-blocking: %s\n",
			    comm_get_error_str(err)));
    }

    _reader = new BufferedAsyncReader(_eventloop, _sock, 4 * 65536,
				      callback(this,
					       &XrlPFSTCPSender::read_event));

    _reader->set_trigger_bytes(STCPPacketHeader::header_size());
    _reader->start();

    _writer = new AsyncFileWriter(_eventloop, _sock, MAX_WRITES);

    _current_seqno   = 0;
    _active_bytes    = 0;
    _active_requests = 0;
    _keepalive_sent  = false;

    // Set the STCP keepalive timeout from environment variable if it is set.
    char* value = getenv("XORP_SENDER_KEEPALIVE_TIME");
    if (value != NULL) {
	char* ep = NULL;
	uint32_t keepalive_s = 0;
	keepalive_s = strtoul(value, &ep, 10);
	if ( !(*value != '\0' && *ep == '\0') &&
	      (keepalive_s <= 0 || keepalive_s > 120)) {
	    XLOG_ERROR("Invalid \"XORP_SENDER_KEEPALIVE_TIME\": %s", value);
	} else {
	    _keepalive_time = TimeVal(keepalive_s, 0);
	}
    }

    if (! _keepalive_time.is_zero())
	start_keepalives();
    sender_list.add_instance(_uid);
}

XrlPFSTCPSender::~XrlPFSTCPSender()
{
    debug_msg("Direct calls %u Indirect calls %u\n",
	      XORP_UINT_CAST(direct_calls), XORP_UINT_CAST(indirect_calls));
    delete _reader;
    _reader = 0;
    delete _writer;
    _writer = 0;
    if (_sock.is_valid()) {
	comm_close(_sock);
	_sock.clear();
    }
    debug_msg("~XrlPFSTCPSender (%p)\n", this);
    sender_list.remove_instance(_uid);
}

void
XrlPFSTCPSender::die(const char* reason, bool verbose)
{
    XLOG_ASSERT(_sock.is_valid());

    if (verbose)
	XLOG_ERROR("XrlPFSTCPSender died: %s", reason);
    stop_keepalives();

    delete _reader;
    _reader = 0;

    _writer->flush_buffers();
    delete _writer;
    _writer = 0;

    comm_close(_sock);
    _sock.clear();

    // Detach all callbacks before attempting to invoke them.
    // Otherwise destructor may get called when we're still going through
    // the lists of callbacks.
    list<ref_ptr<RequestState> > tmp;
    tmp.splice(tmp.begin(), _requests_waiting);
    tmp.splice(tmp.begin(), _requests_sent);

    _active_requests = 0;
    _active_bytes    = 0;

    // Make local copy of uid in case "this" is deleted in callback
    uint32_t uid = _uid;

    while (tmp.empty() == false) {
	if (sender_list.valid_instance(uid) == false)
	    break;
	ref_ptr<RequestState>& rp = tmp.front();
	if (rp->cb().is_empty() == false)
	    rp->cb()->dispatch(XrlError::SEND_FAILED(), 0);
	tmp.pop_front();
    }
}

bool
XrlPFSTCPSender::send(const Xrl&	x,
		      bool		direct_call,
		      const		XrlPFSender::SendCallback& cb)
{
    if (direct_call) {
	direct_calls ++;
    } else {
	indirect_calls ++;
    }

    if (!_sock.is_valid()) {
	debug_msg("Attempted send when socket is dead!\n");
	if (direct_call) {
	    return false;
	} else {
	    cb->dispatch(XrlError(SEND_FAILED, "socket dead"), 0);
	    return true;
	}
    }

    if (direct_call) {
	// We don't want to accept if we are short of resources
	if (_active_requests >= MAX_ACTIVE_REQUESTS) {
	    debug_msg("too many requests %u\n",
		      XORP_UINT_CAST(_active_requests));
	    return false;
	}
	if (x.packed_bytes() + _active_bytes > MAX_ACTIVE_BYTES) {
	    debug_msg("too many bytes %u\n",
		      XORP_UINT_CAST(x.packed_bytes()));
	    return false;
	}
    }

    debug_msg("Seqno %u send %s\n", XORP_UINT_CAST(_current_seqno),
	      x.str().c_str());

    RequestState* rs = new RequestState(this, _current_seqno++,
					_batching, x, cb);
    send_request(rs);

    xassert(_requests_waiting.size() + _requests_sent.size() == _active_requests);

    return true;
}

void
XrlPFSTCPSender::send_request(RequestState* rs)
{
    _requests_waiting.push_back(rs);
    _active_bytes += rs->size();
    _active_requests ++;
    _writer->add_buffer(rs->buffer(), rs->size(),
			callback(this, &XrlPFSTCPSender::update_writer));

    if ((!_batching || rs->is_keepalive()) && _writer->running() == false)
	_writer->start();
}

void
XrlPFSTCPSender::dispose_request()
{
    assert(_requests_sent.empty() == false);
    xassert(_requests_sent.size() + _requests_waiting.size() == _active_requests);
    _active_bytes -= _requests_sent.front()->size();
    _active_requests -= 1;
    _requests_sent.pop_front();
    xassert(_requests_waiting.size() == _writer->buffers_remaining());
}

bool
XrlPFSTCPSender::sends_pending() const
{
    return (_requests_waiting.empty() == false) ||
	(_requests_sent.empty() == false);
}

void
XrlPFSTCPSender::update_writer(AsyncFileWriter::Event	e,
			       const uint8_t*		/* buffer */,
			       size_t			buffer_bytes,
			       size_t			bytes_done)
{
    UNUSED(buffer_bytes);

    if (e == AsyncFileWriter::FLUSHING)
	return; // Code predates FLUSHING

    if (e != AsyncFileWriter::DATA) {
	die("write failed");
    }

    if (bytes_done != buffer_bytes) {
	return;
    }

    ref_ptr<RequestState> rrp = _requests_waiting.front();
    _requests_sent.push_back(rrp);
    _requests_waiting.pop_front();
}

void
XrlPFSTCPSender::read_event(BufferedAsyncReader*	/* reader */,
			    BufferedAsyncReader::Event	ev,
			    uint8_t*			buffer,
			    size_t			buffer_bytes)
{
    debug_msg("read event %u (need at least %u)\n",
		XORP_UINT_CAST(buffer_bytes),
		XORP_UINT_CAST(STCPPacketHeader::header_size()));

    if (ev == BufferedAsyncReader::OS_ERROR) {
	XLOG_ERROR("Read failed (error = %d)\n", _reader->error());
	die("read error");
	return;
    }

    if (ev == BufferedAsyncReader::END_OF_FILE) {
	die("end of file", false);
	return;
    }

    defer_keepalives();

    if (buffer_bytes < STCPPacketHeader::header_size()) {
	// Not enough data to even inspect the header
	size_t new_trigger_bytes = STCPPacketHeader::header_size() - buffer_bytes;
	_reader->set_trigger_bytes(new_trigger_bytes);
	return;
    }

    const STCPPacketHeader sph(buffer);

    if (sph.is_valid() == false) {
	die("bad header");
	return;
    }

    if (sph.seqno() != _requests_sent.front()->seqno()) {
	die("Bad sequence number");
	return;
    }

    if (sph.type() == STCP_PT_HELO_ACK) {
	debug_msg("Got keep alive ack\n");
	_keepalive_sent = false;
	dispose_request();
	_reader->dispose(sph.frame_bytes());
	_reader->set_trigger_bytes(sph.header_size());
	return;
    }

    if (sph.type() != STCP_PT_RESPONSE) {
	die("unexpected packet type - not a response");
    }

    debug_msg("Frame Bytes %u Available %u\n",
	      XORP_UINT_CAST(sph.frame_bytes()),
	      XORP_UINT_CAST(buffer_bytes));
    if (sph.frame_bytes() > buffer_bytes) {
	if (_reader->reserve_bytes() < sph.frame_bytes())
	    _reader->set_reserve_bytes(sph.frame_bytes());
	_reader->set_trigger_bytes(sph.frame_bytes() - buffer_bytes);
	return;
    }

    const uint8_t* xrl_data = buffer + STCPPacketHeader::header_size();

    XrlError xrl_error;
    if (sph.error_note_bytes()) {
	xrl_error = XrlError(XrlErrorCode(sph.error_code()),
			     string((const char*)xrl_data,
				    sph.error_note_bytes()));
	xrl_data += sph.error_note_bytes();
    } else {
	xrl_error = XrlError(XrlErrorCode(sph.error_code()));
    }

    // Get ref_ptr to callback from request state and discard the rest
    XrlPFSender::SendCallback cb = _requests_sent.front()->cb();
    dispose_request();

    xassert(_active_requests == _requests_waiting.size() + _requests_sent.size());

    // Attempt to unpack the Xrl Arguments
    XrlArgs  xa;
    XrlArgs* xap = NULL;
    try {
	if (sph.payload_bytes() > 0) {
	    xa.unpack(xrl_data, sph.payload_bytes());
	    xap = &xa;
	}
    } catch (...) {
	xrl_error = XrlError(XrlError::INTERNAL_ERROR().error_code(),
			     "corrupt xrl response");
	xap = 0;
	debug_msg("Corrupt response: %s\n", xrl_data);
    }

    // Update reader to say we're done with this block of data and
    // specify minimum reserve for next call.
    _reader->dispose(sph.frame_bytes());
    _reader->set_trigger_bytes(STCPPacketHeader::header_size());

    // Dispatch Xrl and exit
    cb->dispatch(xrl_error, xap);

    debug_msg("Completed\n");
}

const char*
XrlPFSTCPSender::protocol() const
{
    return _protocol;
}

// ----------------------------------------------------------------------------
// Sender keepalive related

void
XrlPFSTCPSender::set_keepalive_time(const TimeVal& time)
{
    _keepalive_time = time;
    if (! _keepalive_time.is_zero()) {
	start_keepalives();
    } else {
	stop_keepalives();
    }
}

void
XrlPFSTCPSender::start_keepalives()
{
    _keepalive_timer = _eventloop.new_periodic(
	_keepalive_time,
	callback(this, &XrlPFSTCPSender::send_keepalive),
	XorpTask::PRIORITY_XRL_KEEPALIVE);
}

void
XrlPFSTCPSender::stop_keepalives()
{
    _keepalive_timer.unschedule();
}

void
XrlPFSTCPSender::defer_keepalives()
{
    if (_keepalive_timer.scheduled()) {
	_keepalive_timer.schedule_after(_keepalive_time,
					XorpTask::PRIORITY_XRL_KEEPALIVE);
    }
}

bool
XrlPFSTCPSender::send_keepalive()
{
    TimeVal now;

    _eventloop.current_time(now);
    if (now - _keepalive_last_fired < _keepalive_time) {
	// This keepalive timer has fired too soon. Rate-limit it.
	debug_msg("Dropping keepalive due to rate-limiting\n");
	return true;
    }
    if (_keepalive_sent == true) {
	// There's an unack'ed keepalive message.
	die("Keepalive timeout");
	return false;
    }
    debug_msg("Sending keepalive\n");
    _keepalive_sent = true;
    send_request(new RequestState(this, _current_seqno++));

    // Record the relative timestamp of this keepalive timer firing
    // so that we may rate-limit it above.
    _keepalive_last_fired = now;

    return true;
}

void
XrlPFSTCPSender::batch_start()
{
    _batching = true;
}

void
XrlPFSTCPSender::batch_stop()
{
    _batching = false;

    // If we aint got no requests, we may not be able to signal to the receiver
    // to stop batching and we could deadlock.  In this case we should probably
    // send a keep-alive to resume things.  -sorbo
    XLOG_ASSERT(_requests_waiting.size());

    _requests_waiting.back()->set_batch(false);

    if (_writer->running() == false)
	_writer->start();
}
