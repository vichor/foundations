// Foundations library (fndts): MutexThread class definintion -*- C++ -*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file is part of the Dynasteis game. This library is intended for 
// personal use only; you cannot redistribute it and/or use it in your own 
// program.

/**
 *  \file MutexThread.h
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief The %MutexThread class header file.
**/

/* Avoid multiple inclusions */
#pragma once

/* Include files */
#include "misc/Mutex.h"
#include <pthread.h>

/* Namespace definition and forward declarations */
namespace fndts { namespace os { class MutexThread; } }

/**
 *  \ingroup fndts
 *  \brief  A mutex class appliable to Thread objects.
**/
class fndts::os::MutexThread : public fndts::Mutex
{
    protected:
        pthread_mutex_t mutex;

    public:
        /**
         *  \brief  Creates a mutex object to be used in a thread.
         *
         *  Initializes the OS structures to create a new mutex device.
        **/
        MutexThread();

        /**
         *  \brief  Destroys a %MutexThread object.
        **/
        virtual ~MutexThread();

        /**
         *  \brief  Locks the mutex object.
         *  \return 0 on success; otherwise, the error code.
        **/
        virtual int lock();

        /**
         *  \brief  Unlocks the mutex object.
         *  \return 0 on success; otherwise, the error code.
        **/
        virtual int unlock();
};
