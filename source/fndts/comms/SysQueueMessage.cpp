// Communications library (COMMS): SysQueueMessage class implementation -*- C++-*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file is part of the RoW:D game. This library is intended for personal
// use only; you cannot redistribute it and/or use it in your own program.

/**
 *  \file SysQueueMessage.cpp
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief The %SysQueueMessage class implementation file.
**/

#include <string.h> // for memcpy
#include "SysQueueMessage.h"

using namespace fndts::comms;

/* -- Static member initialization ------------------------------------------ */

/* -- Object methods -------------------------------------------------------- */

/* -- Class methods --------------------------------------------------------- */

/* -- Constructors ---------------------------------------------------------- */

// Public constructor: SysQueueMessage
// Creates an empty queue message
SysQueueMessage::SysQueueMessage()
:
    /* Attribute construction */
    messagetype(0),

    /* Superclass construction */
    Message()
{
}

// Public constructor: SysQueueMessage
// Creates a queue message with the given type
SysQueueMessage::SysQueueMessage(const long type)
:
    /* Attribute construction */
    messagetype(type),

    /* Superclass construction */
    Message()
{
}

// Public constructor: SysQueueMessage
// Creates a queue message with the given initial data
SysQueueMessage::SysQueueMessage(const long type, const size_t sz, const tByte *array)
:
    /* Attribute construction */
    messagetype(type),

    /* Superclass construction */
    Message(sz,array)
{
}

/* -- Destructor ------------------------------------------------------------ */

// Public destructor: ~SysQueueMessage
// Does nothing. The deallocation is performed at superclass level.
SysQueueMessage::~SysQueueMessage()
{
}
