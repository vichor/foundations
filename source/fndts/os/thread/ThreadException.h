// Foundations library (fndts): Thread Exception -*- C++ -*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file is part of the Dynasties game. This library is intended for
// personal use only; you cannot redistribute it and/or use it in your own 
// program.


/** 
 *  \file ThreadException.h
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief The ThreadException class definition file.
**/

/* Avoid multiple inclusions */
#pragma once

/* Include files */
#include <string>
#include "misc/Exception.h"

/* Namespace definition and forward declarations */
namespace fndts { namespace os {
    class Thread;
    class ThreadException; 
} }

/**
 *  \ingroup fndts
 *  \brief  An exception raised from within the thread management classes.
 *
 *  This exception derives from fndts::Exception. It contains a reference
 *  to the Thread object that caused the exception. 
 *
 *  Although the internal reference to the offender object can be obtained,
 *  be aware that it can be an invalid value (such as null).
**/
class fndts::os::ThreadException : public fndts::Exception
{
    private:
        const fndts::os::Thread * object;

    public:

        /*@{*/
        /**
         *  \brief Creates a new %ThreadException 
        **/
        ThreadException() throw();
        ThreadException(const std::string & r) throw();
        ThreadException(const char *r) throw();
        ThreadException(const fndts::os::Thread * o) throw();
        ThreadException(const fndts::os::Thread * o, const std::string & r) throw();
        ThreadException(const fndts::os::Thread * o, const char *r) throw();
        /*@}*/

        /**
         *  \brief  Destroys a %ThreadException object
        **/
        virtual ~ThreadException() throw();

        /**
         *  \brief  Gets the reason that caused the exception and informs about
         *          the thread that caused it if possible.
         *  \return The reason that caused the exception in a null terminated
         *          string format.
        **/
        virtual const char* what() const throw();

        /**
         *  \brief  Gets a pointer to the Thread object that caused the
         *          exception.
         *  \return A pointer to the Thread object.
        **/
        const inline fndts::os::Thread * const getThread() const throw()
        { return object; }
};
