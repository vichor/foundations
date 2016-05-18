// Foundations library (fndts): Exception class implementation -*- C++ -*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file is part of the Dynasties game. This library is intended for 
// personal use only; you cannot redistribute it and/or use it in your own 
// program.

/**
 *  \file Exception.cpp
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief The Exception class implementation file.
**/

#include <exception>
#include <string>

#include "Exception.h"

using namespace fndts;

/* -- Object methods -------------------------------------------------------- */

// Public object method: what
// Inherited from exception. Describes the reason that caused this exception.
const char * Exception::what() const throw()
{ 
    return reason.c_str(); 
}

/* -- Constructors ---------------------------------------------------------- */

// Public constructor: Exception
// Creates a new Exception.
Exception::Exception() throw() 
:
    /* Supcerclass construction */
    exception(),

    /* Attributes construction */
    reason("No explanation given for this exception.") 
{
}

// Public constructor: Exception
// Creates a new Exception with the given reason.
Exception::Exception(const std::string r) throw()
:
    /* Supcerclass construction */
    exception(),    

    /* Attributes construction */
    reason(r)
{
}

// Public constructor: Exception
// Creates a new Exception with the given reason.
Exception::Exception(const char *r) throw()
:
    /* Supcerclass construction */
    exception(),    

    /* Attributes construction */
    reason(r)
{
}

/* -- Destructor ------------------------------------------------------------ */

// Public destructor: ~Exception
// Destroys an Exception object
Exception::~Exception() throw()
{
}

