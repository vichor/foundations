// Foundations library (os): Thread class implementation -*- C++ -*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file is part of the Dynasties game. This library is intended for 
// personal use only; you cannot redistribute it and/or use it in your own 
// program.

/**
 *  \file Thread.cpp
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief The %Thread class implementation file.
**/

#include <string>
#include <map>
#include <pthread.h>
#include <signal.h>
#include "Thread.h"
#include "ThreadException.h"

/* The main program is treated as a fake thread and this is its name */
#define FAKE_THREAD_NAME "__main" 

/* Default namespace */
using namespace fndts::os;

/* -- Static member initialization ------------------------------------------ */
std::map<std::string,Thread*> Thread::threads;
MutexThread Thread::mutex; 

/* 
 * This class is used to report a thread when no one is active. It represents
 * the main thread of the program.
*/
class __os_FakeThread : public Thread
{
    protected: virtual void * threadStartRoutine (void *arg) { /* empty */   }
    public:    __os_FakeThread():Thread(FAKE_THREAD_NAME) { running=true; }
               virtual const int join()                      { /* empty */   }
};
__os_FakeThread __fake;

/* -- Object methods -------------------------------------------------------- */

// Public object method: launch
// Launches the execution of this thread creating it physically in the OS.
const bool Thread::launch(void *arg)
{
    mutex.lock();
    if (!running)
    {
        threadData.thread = this;
        threadData.arg = arg;
        id = pthread_create(&systhread, NULL, onStartThread,&threadData);
        running = true;
        mutex.unlock();
        return true;
    }
    mutex.unlock();
    return false;
}

// Public object method: join
// Waits for the thread to end and stores and returns the thread's return value.
// It also marks the Thread as not being run.
const int Thread::join()
{
    if (running)
    {
        retvalue = pthread_join(systhread, NULL);
        running = false;
    }
    return retvalue;
}

/* -- Class methods --------------------------------------------------------- */

// Public class method: exists
// Looks in the map if a name is in use. Then returns true if so, or false
// otherwise.
bool Thread::exists(const std::string & n)
{
    bool result;
    mutex.lock();
    result = (threads.find(n) != threads.end());
    mutex.unlock();
    return result;
}

// Public class method: getInstance
// Returns a reference to a thread with the given name. If the name does not
// exist, it raises a ThreadException.
Thread & Thread::getInstance(const std::string & n) throw(ThreadException &)
{
    /* Check if the instance exists */
    if (!exists(n))
    {
        std::string s;
        throw ThreadException (s +"Trying to get the instance of the "
                               "inexisting " + n + " thread name.");
    }

   /* Get the instance from the map and return it */
   return * threads[n];
}

// Public class method: getSelf
// Finds and returns the reference to the current thread.
// If no thread, it returns a thread representing the main prorgam
Thread & Thread::getSelf() 
{
    std::map<std::string,Thread*>::iterator ite;
    for(ite=threads.begin(); ite!=threads.end(); ite++)
    {
        if( (*ite).second->getSystemThread() == pthread_self() )
            return *(*ite).second;
    }
    return __fake;
}

// Private class method: onStartThread
// This function is the first one called by the OS for a newly created thread.
// It will obtain the method threadStartRoutine from the given parameter and
// execute it, passing to it the arguments.
void * Thread::onStartThread(void *arg) throw()
{
    t_threadData * tdata = (t_threadData *)arg;
    tdata->thread->threadStartRoutine(tdata->arg);
}

/* -- Constructors ---------------------------------------------------------- */

// Public constructor: Thread
// Creates a new thread object with the given name and inserts it in the map
// object.
Thread::Thread(const std::string & n) throw(ThreadException &)
:
    /* Attributes construction */
    name(n),
    running(false),
    retvalue(0),
    id(-1) /* The ID of the thread is set by the OS when the thread is 
              launched. In the meanwhile, it will be -1. */
{
    /* Check the name */
    if (exists(name)) 
    {
        std::string s("The thread ");
        s += n;
        s += " is already in use.";
        throw ThreadException(const_cast<const std::string &>(s));
    }

   /* OK, go on and add the new name to the map */
   threads[name] = this;
}

// Public constructor: Thread
// Creates a new thread object with the given name and inserts it in the map
// object.
Thread::Thread(const char *n) throw(ThreadException &)
:
    /* Attributes construction */
    name(n),
    running(false),
    retvalue(0),
    id(-1) /* The ID of the thread is set by the OS when the thread is 
               launched. In the meanwhile, it will be -1. */
{
    /**
     *  \todo   Ugly: string and char * based constructors do the same.
     *          How can I avoid repeating it? In a private static function?
     *          Probably...
    **/

    /* Check the name */
    if (exists(name)) 
    {
        std::string s("The thread ");
        s += n;
        s += " is already in use.";
        ThreadException excp (const_cast<const Thread *> (&Thread::getSelf()),
                              const_cast<const std::string &> (s));
        throw excp;
    }

   /* OK, go on and add the new name to the map */
   threads[name] = this;
}

/* -- Destructor ------------------------------------------------------------ */

// Public destructor: ~Thread
// Destoys a thread object, stopping it first, and removing it from the map obj.
Thread::~Thread()
{
    /* Stop the thread */
    if ( running && name != FAKE_THREAD_NAME)
    {
        pthread_kill(systhread,SIGTERM);
    }

    /* Remove from the map */
    threads.erase(name);
}

