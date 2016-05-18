// Communications library (COMMS): Message class implementation -*- C++-*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file is part of the RoW:D game. This library is intended for personal
// use only; you cannot redistribute it and/or use it in your own program.

/**
 *  \file   Message.cpp
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief  The %Message class implementation file.
 *  \todo   Copy constructor needed to put a copy inside containers.
 *  \todo   Operator = needed to copy from the containers.
**/

#include <string.h> // for memcpy prototype
#include "Message.h"

using namespace fndts::comms;

/* -- Static member initialization ------------------------------------------ */

/* -- Object methods -------------------------------------------------------- */

// Public method: size
// Returns the size of the data of this message
const size_t Message::size() const
{
    return msgsize;
}

// Public method: toByteArray
// Transforms this message into an array of bytes. The array must be allocated
// previously by the user.
void Message::toByteArray(tByte *array) const
{
    /* Ensure source and dest. are ok */
    if (array == NULL || data == NULL) return;

    /* Copy data */
    memcpy (array,data,msgsize);
}

// Public method: fromByteArray
// Allocates a new array to hold the data stored in the given array.
void Message::fromByteArray(const size_t sz, const tByte *array)
{
    /* Check size */
    if (sz <= 0) return;

    /* Deallocate previous data */
    if (data != NULL) delete []data;

    /* Allocate for new data and copy data */
    msgsize=sz;
    data = new tByte[msgsize];
    memcpy (data,array,msgsize);
}

/* -- Class methods --------------------------------------------------------- */

/* -- Constructors ---------------------------------------------------------- */

// Public constructor: Message
// Creates an empty message
Message::Message()
:
    /* Attribute construction */
    msgsize(0),
    data(NULL)
{
}

// Public constructor: Message
// Creates a message with the given data
Message::Message(const size_t sz, const tByte *array)
:
    /* Attribute construction */
    msgsize(sz),
    data(NULL)
{
    data = new tByte[msgsize];
    if (array != NULL)
    {
        memcpy(data,array,msgsize);
    }
}

// Public constructor: Message
// Copy constructor
Message::Message(const Message & src)
:
    /* Attribute construction */
    msgsize(src.size()),
    data(NULL)
{
    data = new tByte[msgsize];
    if (data != NULL) src.toByteArray(data);
}
Message::Message(Message & src)
:
    /* Attribute construction */
    msgsize(src.size()),
    data(NULL)
{
    data = new tByte[msgsize];
    if (data != NULL) src.toByteArray(data);
}

/* -- Destructor ------------------------------------------------------------ */

// Public destructor: ~Message
// Deallocates the data
Message::~Message()
{
    if (data != NULL) delete []data;
}

/* -- Operators ------------------------------------------------------------- */

// Operator: = (const Message & src)
// Copies from the source
Message & Message::operator = (const Message & src)
{
    /* Copy the size */
    msgsize = src.size();

    /* Free previously allocated data */
    if (data != NULL) delete []data;

    /* Copy the data */
    data = new tByte[msgsize];
    if (data != NULL) src.toByteArray(data);

    return *this;
}

