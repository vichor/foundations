// Communications library (COMMS): SysQueue class implementation -*- C++-*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file is part of the RoW:D game. This library is intended for personal
// use only; you cannot redistribute it and/or use it in your own program.

/**
 *  \file SysQueue.cpp
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief The %SysQueue class implementation file.
**/

#include <sys/ipc.h> 
#include <sys/msg.h>
#include "SysQueue.h"
#include "Message.h"

using namespace fndts::comms;

/* -- Static member initialization ------------------------------------------ */
const char* SysQueue::keyName="./keyfile"; 
const char SysQueue::projectID='A';
const int SysQueue::permissions=0600;

/* -- Object methods -------------------------------------------------------- */

// Public method: close
// Closes the message queue discarding pending messages.
const bool SysQueue::close()
{
    /** 
     *  \todo   Error treatment when closing the queue. Nowadays, we always
     *          return true.
     *  \todo   Check if the queue is closable before actually closing it.
    **/
    msgctl(id, IPC_RMID, NULL);
    return true;
}

// Public method: send
// Sends a message to the queue
const bool SysQueue::send (const Message &m) const
{
    /* Create a temporal SysQueueMessage of type 1 (0 not allowed when sending) */
    tByte        array[m.size()];
    SysQueueMessage tmsg(1l,m.size(),array);

    /* Send a SysQueueMessage */
    send(tmsg);
}

// Public method: send
// Sends a message to the queue
const bool SysQueue::send (const SysQueueMessage &m) const
{
    /** 
     *  \todo   Check type of the message and throw an exception 
     *  \todo   Error management.
    **/

    /* Get the type and data of the message */
    tByte contents[sizeof(long)+m.size()];
    long * ptype = reinterpret_cast<long *>(contents);
    *ptype++ = m.getType();
    m.toByteArray( reinterpret_cast<tByte *>(ptype) );

    /* Actually send the message */
    msgsnd(id,contents,m.size(),0);

    /** \todo   Check result of msgsnd and return true/false **/
    return true;
}

// Public method: receive
// Receives a message from the queue
const bool SysQueue::receive(comms::Message & r)
{
    /* Create a temporal SysQueueMessage with type 0 (allow any type of message) */
    tByte buffer[r.size()];
    SysQueueMessage tmsg(0l,r.size(),buffer);

    /* Receive a SysQueueMessage */
    receive(tmsg);
}


// Public method: receive
// Receives a message from the queue
const bool SysQueue::receive(comms::SysQueueMessage & r)
{
    /** 
     *  \todo   Treat different means of receiving a message: sync, async, 
     *          with timeouts, etc 
     *  \todo   Error management.
    **/

    /* Check not empty destination message */
    if (r.size() == 0) return false;

    /* Allocate locally buffer for the received message */
    int sz = sizeof(long)+r.size();
    tByte buffer[sz];

    /* Pointers to easily access type and data in the buffer */
    long *pType = reinterpret_cast<long *>(buffer);
    tByte *pData = buffer+sizeof(long);

    /* Actually receive the message */
    /** \todo   Check the flags (last parameter of msgrcv call now 0) **/
    msgrcv(id,buffer,r.size(),r.getType(),0);
    /* 
     * Load the buffered message to the object.
     * We use the type received from the queue in case the one stored in the
     * object was 0 (meaning "i want to receive any kind of message"). 
    */
    r.setType(*pType);
    r.fromByteArray(r.size(),pData);

    /** \todo   Check result of msgrcv and return true/false **/
    return true;
}

/* -- Class methods --------------------------------------------------------- */

/* -- Constructors ---------------------------------------------------------- */

// Public constructor: SysQueue
// Creates a system message queue
SysQueue::SysQueue()
:
    /* Attribute construction */
    master(true),

    /* Superclass construction */
    Channel("Message queue")
{
    /* Creates the queue */
    key = ftok(keyName,projectID);
    /** 
     *  \todo   Error treatment when creating the message queue 
     *  \todo   Configure queue parameters with msgctl
    **/
    id =  msgget(key, permissions | IPC_CREAT);
    master=true;
}

// Public constructor: SysQueue
// Creates a new SysQueue object attaching it to the given system queue id.
SysQueue::SysQueue(int qid)
:
    /* Attribute construction */
    id(qid),
    master(false),

    /* Superclass construction */
    Channel("Message queue")
{
}

/* -- Destructor ------------------------------------------------------------ */

// Public desctructor: ~SysQueue
// Closes the message queue
SysQueue::~SysQueue()
{
    if (master) close();
}
