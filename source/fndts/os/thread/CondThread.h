// Foundations library (fndts): CondThread class definintion -*- C++ -*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file is part of the Dynasteis game. This library is intended for 
// personal use only; you cannot redistribute it and/or use it in your own 
// program.

/**
 *  \file CondThread.h
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief The %CondThread class header file.
**/

/* Avoid multiple inclusions */
#pragma once

/* Include files */
#include "MutexThread.h"

/* Namespace definition and forward declarations */
namespace fndts { namespace os { class CondThread; } }

/**
 *  \ingroup fndts
 *  \brief  A condition mutex class appliable to Thread objects.
 *
 * A condition mutex is a mutex object which allows a condition to be associated
 * with a condition. The condition can be signaled to become true (thus, 
 * unlocking) or can be waited until it becomes true (thus, locking). Several
 * threads can be associated with the same condition, so they can wait and
 * signal the condition to synchronize between them.
**/
class fndts::os::CondThread : public fndts::os::MutexThread
{
    private:
        pthread_cond_t condition;

    public:
        /**
         *  \brief  Creates a condition mutex object to be used in a thread.
         *
         *  Initializes the OS structures to create a new condition device.
        **/
        CondThread();

        /**
         *  \brief  Destroys a %CondThread object.
        **/
        virtual ~CondThread();

        /**
         *  \brief  Waits for the condition to become true.
         *  \return The OS result of the call.
        **/
        int wait();

        /**
         *  \brief  Signal this condition to become true to all waiting threads.
         *  \return The OS result of the call.
        **/
        int signal();

        /**
         *  \brief  Signal this condition to become true to just one waiting
         *          thread.
         *  \return The OS result of the call.
        **/
        int signalOne();
};
