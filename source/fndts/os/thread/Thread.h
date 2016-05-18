// Foundations library (fndts): Thread class definintion -*- C++ -*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file is part of the RoW:D game. This library is intended for personal
// use only; you cannot redistribute it and/or use it in your own program.

/**
 *  \file Thread.h
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief The %Thread class header file.
**/

/* Avoid multiple inclusions */
#pragma once

/* Include files */
#include <string>
#include <map>
#include <pthread.h>

#include "ThreadException.h"
#include "MutexThread.h"

/* Namespace definition and forward declarations */
namespace fndts { namespace os { class Thread; } }

/**
 *  \ingroup fndts
 *  \brief  The %Thread class.
 *
 *  This is an abstract class defining the behavior of any object willing to 
 *  become a thread in the current process.
 *
 *  Each thread will have a unique name used to identify itself among other 
 *  threads. This name can be used to get a reference to a given thread and
 *  communicate with it.
 *
 *  The %Thread class maintains an internal list of all created threads, so
 *  they can be searched easily, and provides iterators to this list. Any
 *  object can then iterate through all the created threads and send them
 *  messages (such as finisish yourself or whatever).
 *
 *  In case the main program asks to get a reference to its thread (see the
 *  method Thread::getSelf()) a reference to a fake thread is returned. This
 *  fake thread will have the name \c __main and cannot be launched or joined.
 *  As it is supposed to be already running, the method isRunning() will 
 *  return always \c true.
**/
class fndts::os::Thread
{
    private:
        static std::map<std::string,Thread*> threads; /* all the threads */
        static fndts::os::MutexThread mutex;

        int id;                 /* Thread ID returned by pthread_create */
        const std::string name; /* Name of this thread */
        pthread_t systhread;    /* OS thread object */
        int retvalue;           /* Value returned by the last execution */
    protected:
        bool running;           /* Is this thread running? */

    private:
        /*
         * The function starting a thread will receive a instance of this
         * struct as parameter. The function is common for all the threads,
         * so a pointer to the Thread object (including the implementation of
         * the function threadStartRoutine) is needed.
        */
        typedef struct 
        {
            Thread * thread;
            void * arg;
        } t_threadData;
        t_threadData threadData;

        /**
         *  \brief First function called by the OS calls on the new thread. 
         *  \param  arg Thread data including the %Thread object and the
         *              arguments.
         *  \return <Automatic value>
        **/
        static void * onStartThread (void * arg) throw();

        /* Copy constructor and operator = disabled */
        Thread(Thread & src) {}
        Thread(const Thread & src) {}
        Thread & operator = (const Thread & src) {}
        
    protected:
        /**
         *  \brief The entry point of this thread (Pure virtual method).
         *
         *  When this thread is launched, it will start running from this
         *  function. This method is the function that will be executed when
         *  the kernel creates internally the thread.
         *
         *  \param arg  The parameters received by any thread entry point.
         *  \return The thread result (errno).
        **/
        virtual void * threadStartRoutine(void *arg) = 0;

        /**
         *  \brief Gets the system ID of the instance of the thread.
         *  \return The thread ID.
        **/
        inline const int getSystemID() const
        { return id; }

        /**
         *  \brief Gets the system thread object of the instance of the thread.
         *  \return The thread object.
        **/
        inline pthread_t getSystemThread() const
        { return systhread; }

    public:
        /**@{*/
        /**
         * \brief Creates a new Thread object.
         *
         *  These are the %Thread constructors. As a %Thread has a unique name,
         *  the constructor checks whether this name exists. If so, a number 
         *  will be appended to the name to identify the new created thread.
         *
         *  \param n    The name for the new %Thread object.
         *  \throw ThreadException if the given name is already in use.
         *  \todo Hey!! Maybe I should not be raising an exception if the name 
         *        is in use.
        **/
        Thread(const std::string & n) throw(fndts::os::ThreadException &);
        Thread(const char *n) throw(fndts::os::ThreadException &);
        /**@}*/

        /**
         *  \brief Destoys a %Thread object.
         *
         *  If the thread is running, it stops the thread.
        **/
        virtual ~Thread();

        /**
         *  \brief Launches the execution of this thread.
         *
         *  After a %Thread object has been created and configured properly,
         *  it can be set to execution (run) by calling this method.
         *
         *  In case a problem arises, ThreadException will be rised.
         *
         *  \param arg The parameters for the thread entry point.
         *  \return true when the thread was created successfully; 
         *          false otherwise.
         *
         *  \throw ThreadException on thread creation error.
        **/
        virtual const bool launch(void *arg);

        /**
         *  \brief  Waits for the thread to end.
        **/
        virtual const int join();

        /**
         *  \brief Gets the %Thread name.
         *  \return The %Thread name.
        **/
        inline std::string getName() const
        { return name; }

        /**
         *  \brief Checks if the thread is running.
         *  \return true if the thread is launched (running); false, otherwise.
        **/
        inline bool isRunning() const
        { return running; }

        /**
         *  \brief Checks if a %Thread exists with the given name.
         *  \param  n   Name to check
         *  \return true if it exists; false otherwise.
        **/
        static bool exists(const std::string & n);

        /**
         *  \brief Gets the %Thread instance of the given name.
         *  \param  n   Name of the thread to get. 
         *  \return The %Thread instance.
         *  \throw ThreadException if the given name does not exist.
        **/
        static Thread & getInstance(const std::string & n) 
            throw(ThreadException &);

        /**
         *  \brief  Gets the %Thread instance of the current active thread.
         *  \return A %Thread reference to the current thread.
         *
         *  If no thread is active, this method cannot return a reference so
         *  a fake thread object is created representing the main program.
         *  This fake thread will have the name '__main' and cannot be launched,
         *  nor joined.
        **/
        static Thread & getSelf();
};
