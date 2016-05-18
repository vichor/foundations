// Communications library (COMMS): Channel class implementation -*- C++-*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file is part of the RoW:D game. This library is intended for personal
// use only; you cannot redistribute it and/or use it in your own program.

/**
 *  \file Channel.cpp
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief The %Channel class implementation file.
**/

#include <string>
#include "Channel.h"

using namespace fndts::comms;

/* -- Static member initialization ------------------------------------------ */

/* -- Object methods -------------------------------------------------------- */

/* -- Class methods --------------------------------------------------------- */

/* -- Constructors ---------------------------------------------------------- */

// Protected constructor: Channel
// It just sets the name to the channel and builds the nodes map.
Channel::Channel (const std::string n)
:
    /* Attributes construction */
    name(n)
{
}

// Protected constructor: Channel
// It just sets the name to the channel and builds the nodes map.
Channel::Channel (const char *n)
:
    /* Attributes construction */
    name(n)
{
}

// Private constructor: Channel
// Copy constructor disabled being private
Channel::Channel(const Channel & src)
{
    /** \todo   Raise an exception when used */
}


/* -- Destructor ------------------------------------------------------------ */

// Protected destructor: ~Channel
// Does nothing
Channel::~Channel()
{
}
