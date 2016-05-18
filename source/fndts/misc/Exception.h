// Foundations library (FNDTS): Exception class definintion -*- C++-*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file is part of the RoW:D game. This library is intended for personal
// use only; you cannot redistribute it and/or use it in your own program.


/** 
 *  \file Exception.h
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief The Exception class definition file.
**/

/* Avoid multiple inclusions */
#pragma once

/* Include files */
#include <exception>
#include <string>

/* Namespace definition and forward declarations */
namespace fndts { class Exception; }

/**
 *  \ingroup fndts
 *  \brief A general exception in Foundations library scope.
 *
 *  This exception class derives from std::exception. It contains a string
 *  detailing the reason that caused the exception.
**/
class fndts::Exception : public std::exception
{
    private:
        std::string reason;

    public:
        /**
         *  \brief Creates a new %Exception in the fndts space.
        **/
        /*@{*/
        Exception() throw();
        Exception(const std::string r) throw();
        Exception(const char *r) throw();
        /*@}*/

        /**
         *  \brief  Destroys the %Exception.
        **/
        virtual ~Exception() throw();

        /**
         *  \brief Gets the reason that caused the exception.
         *  Inherited from std::exception class.
         *  \return The reason that caused the exception in a null terminated
         *          string format.
        **/
        virtual const char* what() const throw();
};
