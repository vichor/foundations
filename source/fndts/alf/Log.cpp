// Foundations library (alf-another logging facility): Log -*- C++ -*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file was developed as part of the Dynasties game. This library is 
// intended for personal use only; you cannot redistribute it and/or use it in 
// your own program.


/** 
 *  \file Log.cpp
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief  The ALF Log class implementation file.
**/

#include <string>
#include <string.h> /* for strlen */
#include "Log.h"
#include "comms/Message.h"
#include "os/thread/Thread.h"

using namespace fndts::alf;

/* -- Static member initialization ------------------------------------------ */

/* -- Object methods -------------------------------------------------------- */

// Public object method: toMessage
// Transforms this log into a message.
fndts::comms::Message Log::toMessage() const
{
    /* 
     *  The message is formed by:
     *      - The log ending with the char 0
     *      - The thread name ending with the char 0
     *      - The channel name ending with the char 0
     *      - The level of the log
     *      - The type of log
     */   
    size_t sz = (strlen(this->text.c_str())+1) + 
                (strlen(this->thread.c_str())+1) + 
                (strlen(this->channel.c_str())+1) + 
                sizeof(this->level) + 
                sizeof(this->type);

    /* Allocate buffer for the needed data */
    fndts::comms::tByte buffer[sz];

    /* Iterator through buffer */
    fndts::comms::tByte *buffit = buffer;

    /* Set all data */
    strcpy(reinterpret_cast<char *>(buffit),this->text.c_str()); 
    buffit += strlen(this->text.c_str())+1;
    strcpy(reinterpret_cast<char *>(buffit),this->thread.c_str()); 
    buffit += strlen(this->thread.c_str())+1;
    strcpy(reinterpret_cast<char *>(buffit),this->channel.c_str()); 
    buffit += strlen(this->channel.c_str())+1;
    *(reinterpret_cast<unsigned int *>(buffit)) = this->level; 
    buffit += sizeof(this->level);
    *(reinterpret_cast<eLogType *>(buffit)) = this->type;

    /* Create a message and copy there the data from the buffer */
    fndts::comms::Message msg;
    msg.fromByteArray(sz,buffer);

    /* Return the created message */
    return msg;
}

/* -- Class methods --------------------------------------------------------- */

/* -- Constructors ---------------------------------------------------------- */

// Public constructor: Log
// Creates a new log object with the given level, channel and text.
Log::Log(const eLogType k, const unsigned int l, const char *c, const char *t)
:
    /* Attribute construction */
    text(t),
    channel(c),
    thread(os::Thread::getSelf().getName()),
    level(l),
    type(k)
{
}


// Public constructor: Log
// Creates a new log object with the given level, channel and text.
Log::Log(const eLogType k, const unsigned int l, const std::string &c, 
         const std::string &t)
:
    /* Attribute construction */
    text(t),
    channel(c),
    thread(os::Thread::getSelf().getName()),
    level(l),
    type(k)
{
}

// Public constructor: Log
// Creates a Log from a Message
Log::Log(const fndts::comms::Message src)
{
    /* Store locally the contents of the source message */
    fndts::comms::tByte buffer[src.size()];
    src.toByteArray(buffer);

    /* Iterator through buffer */
    fndts::comms::tByte *buffit = buffer;

    /* Read data from the message and store it in this log */
    this->text = reinterpret_cast<char *>(buffit);
    buffit += strlen(this->text.c_str())+1;
    this->thread = reinterpret_cast<char *>(buffit);
    buffit += strlen(this->thread.c_str())+1;
    this->channel = reinterpret_cast<char *>(buffit);
    buffit += strlen(this->channel.c_str())+1;
    this->level = *(reinterpret_cast<unsigned int *>(buffit));
    buffit += sizeof(this->level);
    this->type = *(reinterpret_cast<eLogType *>(buffit));
}

/* -- Destructor ------------------------------------------------------------ */

// Public destructor: ~Log
// Destroys the Log object
Log::~Log()
{
}

/* -- Operators related with Log class -------------------------------------- */

