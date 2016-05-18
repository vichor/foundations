// Foundations library (os): CondThread class implementation -*- C++ -*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file is part of the Dynasteis game. This library is intended for 
// personal use only; you cannot redistribute it and/or use it in your own 
// program.

/**
 *  \file CondThread.cpp
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief The %CondThread class implementation file.
**/

#include <pthread.h>
#include "CondThread.h"
#include "MutexThread.h"

using namespace fndts::os;

/* -- Static member initialization ------------------------------------------ */

/* -- Object methods -------------------------------------------------------- */

// Public Method: wait
// Waits this condition.
int CondThread::wait()
{
    return pthread_cond_wait (&condition,&mutex);
}

// Public Method: signal
// Signals all the waiting threads for this condition.
int CondThread::signal()
{
    return pthread_cond_broadcast (&condition);
}

// Public Method: signalOne
// Signals one of the waiting threads for this condition.
int CondThread::signalOne()
{
    return pthread_cond_signal (&condition);
}

/* -- Class methods --------------------------------------------------------- */

/* -- Constructors ---------------------------------------------------------- */

// Public constructor: CondThread
// Creates the needed OS structures and initializes a condition mutex object.
CondThread::CondThread()
:
    /* Superclass construction */
    MutexThread()
{
    pthread_cond_init(&condition,NULL);
}

/* -- Destructor ------------------------------------------------------------ */

// Public destructor: ~CondThread
// Destroys the condition mutex.
CondThread::~CondThread()
{
}
