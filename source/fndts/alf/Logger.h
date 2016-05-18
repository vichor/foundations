// Foundations library (alf-another logging facility): Logger class -*- C++ -*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file was developed as part of the Dynasties game. This library is 
// intended for personal use only; you cannot redistribute it and/or use it in 
// your own program.

/** 
 *  \file Logger.h
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief  The Logger class header file.
**/

/* Avoid multiple inclusions */
#pragma once

/* Include files */
#include "os/thread/Thread.h"
#include "os/thread/MutexThread.h"
#include "comms/Channel.h"
#include <ostream>
#include <map>

/* Namespace definition and forward declarations */
namespace fndts { namespace alf { 
    class Logger; 
    class Log; 
    class LogChannel; 
} }

/** 
 *  \ingroup alf
 *  \brief The global %Logger supervisor for the ALF library. 
 *
 *  The %Logger class has two functions within the ALF library:
 *
 *      - be a manager for the logging facility, and
 *      - issue errors, warnings and logs to the standard error and logging.
 *
 *  \section ALF-LOGGER-1 Logger as manager
 *  The %Logger class will host all the created LogChannel and it is also used
 *  to open or close them.
 *
 *  There can be only one %Logger per program. This is controlled internally in 
 *  the %Logger class. To achieve this, the constructor is invisible to the 
 *  user. Instead of creating directly new %Logger object, the user needs to 
 *  call the getLogger() method, which takes one argument:
 *
 *      - the global log level for the %Logger.
 *
 *  This global log level will be used to filter out the received logs whose log
 *  level is lower than this global level.
 *
 *  The %Logger is run in its own thread (driven by a Thread instance). When it
 *  is no longer needed, it needs to be stopped calling the close() method.
 *
 *  \section ALF-LOGGER-2 Logger as Log reporter
 *  Communications between the %Logger object and the LogChannel object is 
 *  performed through Log objects sent through a common Queue. These Log objects
 *  will contain a type identifying the nature of the log:
 *
 *      - eEXIT, to ask the %Logger thread to end.
 *      - eERROR, to ask the %Logger thread to issue an error report.
 *      - eWARNING, to ask the %Logger thread to issue a warning report.
 *      - eSTANDARD, to ask the %Logger thread to issue an standard log report.
 *  
 *  Both error and warning reports will only arise in the output when their 
 *  flag is active. Use the methods setErrorFlag() and setWarningFlag() to 
 *  control these flags.
 *
 *  Standard log reports will only arise if their level is equal or greater than
 *  the %Logger's global log level.
 *
 *  When a waring or error is issued, it is directed towards the standard error
 *  stream (std::cerr); standard logs are directed towards the standard logging 
 *  stream (std::clog).
 *
 *  \section Using Logger in multithreaded programs
 *  This class is ready to be used in multithreaded programs (in fact, its 
 *  engine runs in its own thread). 
 *
 *  All issued logs will be written atomically to the output, avoiding logs 
 *  being requested from different threads to overlap.
 *
**/
class fndts::alf::Logger : public fndts::os::Thread
{
    private:
        /* LogChannels container */
        static std::map<std::string,LogChannel*> channels; 
        static fndts::comms::Channel  *ioport; /* IO port for receiving logs */
        static fndts::os::MutexThread mutex; /* Mutex for the object members */
        static Logger * singleton;    /* Singleton pattern driver */
        static unsigned int glevel;   /* Global log level */
        static bool eflag, wflag;     /* error and warning flags to filter */

        /** 
         *  Creates new %Logger object with the given global log level.
        **/
        Logger(unsigned int l);

        /** 
         *  When destroying a %Logger object, the thread will be destroy and
         *  all its associated LogChannel will be destroyed as well, leaving 
         *  the whole ALF inoperative.
        **/
        virtual ~Logger();
        
        /* Methods to manage messages received from LogChannels */
        void exit (const Log & log) const;
        void error (const Log & log) const;
        void warning (const Log & log) const;
        void standard (const Log & log) const;
        void report(std::ostream & out, const Log & log) const;

    public:
        /**
         *  \brief  Checks if a given LogChannel exists.
         *  \param  n   Name of the LogChannel.
         *  \return true if the LogChannel exists; false otherwise
        **/
        const bool logChannelExists(const std::string n) const;

        /**
         *  \brief The thread task for the %Logger object.
         *
         *  When the logger is created, a thread is launched (see constructor)
         *  This function is the entry point of the thread and implements the
         *  message reception and treatment loop.
         *
         *  \param arg  Thread's parameter (no parameter expected).
         *  \return The thread result (errno).
        **/
        virtual void * threadStartRoutine(void *arg);

         /**
          *  \brief  Gets the communications port/channel.
          *  \return The %Logger's channel.
         **/
         static inline fndts::comms::Channel & getIOPort()
         { return *ioport; }

        /** 
         *  \brief Sets the global log level of the %Logger object. 
         *  \param l New global log level for the %Logger object.
        **/
        static void setGlobalLogLevel(unsigned int l);

        /** 
         * \brief Returns the global log level of the %Logger object. 
         * \return An unsigned integer containing the global log level.
        **/
        static unsigned int getGlobalLogLevel();

        /**
         *  \brief  Gets the error flag.
         *  \return The error flag.
        **/
        static const bool getErrorFlag();

        /**
         *  \brief  Sets the error flag.
         *  \param  e   New error flag.
        **/
        static void setErrorFlag(const bool e);

        /**
         *  \brief  Gets the warning flag.
         *  \return  The warning flag.
        **/
        static const bool getWarningFlag();

        /**
         *  \brief  Sets the warning flag.
         *  \param  w   New warning flag.
        **/
        static void setWarningFlag(const bool w);

        /**
         *  \brief Open/creates a log channel.
         *
         *  Creates a new log channel or, if the name already exists, obtain
         *  the reference to the currently existing one.
         *
         *  \param n Name of the log channel.
         *  \param l Log level of the log channel. Optional, default to 0.
         *
         *  \return A reference to the (perhaps newly created) log channel.
        **/
        LogChannel & openLogChannel(const std::string n, 
                                    const unsigned int l=0);

        /**
         *  \brief Remove and destroy a log channel.
         *
         *  Searches for the log channel name in the internal list and, if
         *  found, destroys the log channel and removes it from the internal
         *  list. Any external references to the log channel destroyed will 
         *  be invalid and may unexpected behavior.
         *
         *  \param n The name of the channel to remove.
         *
         *  \return True if the log channel was successfully removed.
        **/
        const bool closeLogChannel(const std::string n);

        /** 
         *  \brief Creates a new %Logger thread or returns the reference to the 
         *         current %Logger object.
         *
         *  The %Logger class implements a singleton pattern. Thus, only one
         *  instance of the class may exist at one instant. In case an instance
         *  already exists, this method will return a reference to it; if it
         *  exists not, a new %Logger object is created and its reference 
         *  returned.
         *
         *  \param l New global log level to set to the new %Logger object.
         *           Optional, default to 0.
         *
         *  \return A pointer to the new or current %Logger object.
        **/
        static Logger & getLogger(unsigned int l=0);

        /**
         *  \brief  Closes ALF.
         *  \return true when successful; false otherwise.
        **/
        static const bool close();

        /** 
            \brief Returns the version of the ALF library. 
            \return A pointer to a char array containing the ALF version.
        **/
        static const char * getVersion();

        /**
         *  \brief ALF4C++ debugging method.
         *
         *  Shows information about the %Logger object and all its log channels.
         *  This is intended only to check ALF behavior and should not be used
         *  by users.
        **/
        static void debug();
};
