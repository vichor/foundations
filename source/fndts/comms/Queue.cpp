// Communications library (COMMS): Queue class implementation -*- C++-*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file is part of the RoW:D game. This library is intended for personal
// use only; you cannot redistribute it and/or use it in your own program.

/**
 *  \file   Queue.cpp
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief  The %Queue class implementation file.
 *  \todo   Check mutex in the whole implementation
**/

#include "Queue.h"
#include "Message.h"
#include <map>

using namespace fndts::comms;

/* -- Static member initialization ------------------------------------------ */
std::map<unsigned int,Queue*> Queue::qlist;
fndts::os::MutexThread Queue::gmutex;

/* -- Object methods -------------------------------------------------------- */

// Public method: close
// Closes the queue discarding pending messages.
const bool Queue::close()
{
    mutex.lock();
    while (!q.empty())
        q.pop();
    mutex.unlock();
    return true;
}

// Public method: send
// Sends a message to the queue
const bool Queue::send (const Message &m) 
{
    msgavail.lock(); 
    q.push(m);
    msgavail.signal(); 
    msgavail.unlock(); 
    return true;
}

// Public method: receive
// Waits for a message in the queue and copies it in the parameter
const bool Queue::receive(comms::Message & r)
{
    /* When no message available, wait for one */
    msgavail.lock(); 
    if (q.empty())
    {
        msgavail.wait();
    }
    msgavail.unlock(); 
         
    /* Get the message */
    mutex.lock();
    r = q.front();
    q.pop();
    mutex.unlock();
    return true;
}

/* -- Class methods --------------------------------------------------------- */

// Public method: exists
// Checks if a queue with the given ID exists
const bool Queue::exists(unsigned int n)
{
    gmutex.lock();
    bool r=(qlist.find(n) != qlist.end());
    gmutex.unlock();
    return r;
}

// Public function: getQueue
// Gets a reference to the asked Queue
Queue * Queue::getQueue(unsigned int n)
{
    Queue * pq = NULL;
    if (exists(n))
    {
        gmutex.lock();
        pq = qlist[n];
        gmutex.unlock();
    }
    return pq;
}

/* -- Constructors ---------------------------------------------------------- */

// Public constructor: Queue
// Creates a system message queue
Queue::Queue()
:
    /* Attribute construction */
    id(0),
    q(),
    msgavail(),
    mutex(),

    /* Superclass construction */
    Channel("FIFO Queue")
{
    /* Get a new ID */
    unsigned int nid;
    for (nid=0; exists(nid); nid++)
            ;
    id=nid;

    /* Add the fifo queue to the internal list */
    gmutex.lock();
    Queue::qlist[id]=this;
    gmutex.unlock();
}

/* -- Destructor ------------------------------------------------------------ */

// Public desctructor: ~Queue
// Closes the queue
Queue::~Queue()
{
    close();
    gmutex.lock();
    qlist.erase(id);
    gmutex.unlock();
}


/* -- Operators ------------------------------------------------------------- */

