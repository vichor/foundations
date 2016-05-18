// Foundations library (os): MutexThread class implementation -*- C++ -*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file is part of the Dynasteis game. This library is intended for 
// personal use only; you cannot redistribute it and/or use it in your own 
// program.

/**
 *  \file MutexThread.cpp
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief The %MutexThread class implementation file.
**/

#include <pthread.h>
#include "MutexThread.h"

using namespace fndts::os;

/* -- Static member initialization ------------------------------------------ */

/* -- Object methods -------------------------------------------------------- */

// Public Method: lock
// Locks this mutex.
int MutexThread::lock()
{
    int r = pthread_mutex_lock(&mutex);
    return r;
}

// Public Method: unlock
// Unlocks this mutex.
int MutexThread::unlock()
{
    return pthread_mutex_unlock(&mutex);
}

/* -- Class methods --------------------------------------------------------- */

/* -- Constructors ---------------------------------------------------------- */

// Public constructor: MutexThread
// Creates the needed OS structures and initializes a thread mutex object.
MutexThread::MutexThread()
{
   pthread_mutexattr_t attr;

   pthread_mutexattr_init(&attr);
   pthread_mutex_init(&mutex,&attr);
}

/* -- Destructor ------------------------------------------------------------ */

// Public destructor: ~MutexThread
// 
/** \todo   Put description for destructor. **/
MutexThread::~MutexThread()
{
    /**
     *  \todo   Destructor body.
    **/
}
