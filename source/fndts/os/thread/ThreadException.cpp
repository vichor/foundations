// Foundations library (os): Namespace definintion -*- C++ -*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file is part of the RoW:D game. This library is intended for personal
// use only; you cannot redistribute it and/or use it in your own program.

/**
 *  \file ThreadException.cpp
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief The ThreadException class implementation file.
**/

#include <string>

#include "ThreadException.h"
#include "misc/Exception.h"
#include "Thread.h"

using namespace fndts::os;

/* -- Object methods -------------------------------------------------------- */

// Public object method: what
// Inherited from Exception. Describes the reason that caused this exception.
// If able to, it adds information about the offender thread.
const char * ThreadException::what() const throw()
{
    std::string s(Exception::what());
    try
    {
        if (object != NULL)
        {
            s += " raised from the thread ";
            s += object->getName();
        }
    } catch(...) { }
    return s.c_str();
}

/* -- Constructors ---------------------------------------------------------- */

// Public constructor: ThreadException
// Creates a new ThreadException.
ThreadException::ThreadException() throw()
:
    /* Supcerclass construction */
    Exception()
{
    object=NULL;
}

// Public constructor: ThreadException
// Creates a new ThreadException.
ThreadException::ThreadException(const std::string & r) throw()
:
    /* Supcerclass construction */
    Exception(r)
{
    object=NULL;
}

// Public constructor: ThreadException
// Creates a new ThreadException.
ThreadException::ThreadException(const char *r) throw()
:
    /* Supcerclass construction */
    Exception(r)
{
    object=NULL;
}

// Public constructor: ThreadException
// Creates a new ThreadException.
ThreadException::ThreadException(const Thread * o) throw()
:
    /* Supcerclass construction */
    Exception(),

    /* Attributes construction */
    object(o)
{
}

// Public constructor: ThreadException
// Creates a new ThreadException.
ThreadException::ThreadException(const Thread * o, const std::string &r) throw()
:
    /* Supcerclass construction */
    Exception(r),

    /* Attributes construction */
    object(o)
{
}

// Public constructor: ThreadException
// Creates a new ThreadException.
ThreadException::ThreadException(const Thread * o, const char * r) throw()
:
    /* Supcerclass construction */
    Exception(r),

    /* Attributes construction */
    object(o)
{
}

/* -- Destructor ------------------------------------------------------------ */

// Public destructor: ~ThreadException
// Destroys a ThreadException object
ThreadException::~ThreadException() throw()
{
}

