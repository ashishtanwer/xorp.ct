// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2001-2003 International Computer Science Institute
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

// $XORP: xorp/rip/update_queue.hh,v 1.1 2003/04/10 00:27:44 hodson Exp $

#ifndef __RIP_UPDATE_QUEUE__
#define __RIP_UPDATE_QUEUE__

#include <vector>
#include "route_db.hh"

/**
 * @short Update Queue for RIP Route entries.
 *
 * The Update Queue has is conceptually a single writer multi-reader
 * queue.  It is used to store state for triggered updates and may be
 * used unsolicited responses (routing table announcements).
 */
template <typename A>
class UpdateQueue {
public:
    struct Reader;
    struct ReaderPool;
    typedef ref_ptr<Reader> ReadIterator;
    typedef ref_ptr<const RouteEntry<A> > RouteUpdate;
    typedef vector<RouteUpdate> Queue;

public:
    UpdateQueue();
    ~UpdateQueue();

    /**
     * Add update to back of queue.
     */
    void push_back(const RouteUpdate& ru);

    /**
     * Remove all queued entries and reset all read iterators to the front
     * of the queue.
     */
    void flush();
    
    /**
     * Create a read iterator.  These are reference counted entities that
     * need to be stored in order to operate.
     */
    ReadIterator create_reader();

    /**
     * Destroy read iterator.  This method detaches the iterator from the
     * update queue.  Use of the iterator after this call is unsafe.
     */
    void destroy_reader(ReadIterator& r);
    
    /**
     * Increment iterator and return pointer to entry if available.
     *
     * @return A pointer to a RouteEntry if available, 0 otherwise.
     */
    const RouteEntry<A>* next(ReadIterator& r) const;

    /**
     * Get the RouteEntry associated with the read iterator.
     *
     * @return A pointer to a RouteEntry if available, 0 otherwise.
     */
    const RouteEntry<A>* get(ReadIterator& r) const;

    /**
     * Advance read iterator to end of update queue.  Calls to
     * @ref next and @ref get will return 0 until further
     * updates occur.
     */
    void ffwd(ReadIterator& r) const;
    
protected:
    struct ReaderPool* _pool;
    Queue   _queue;
};

#endif // __RIP_UPDATE_QUEUE__
