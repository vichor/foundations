// Foundations library (alf-another logging facility): Loggger -*- C++ -*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file was developed as part of the Dynasties game. This library is 
// intended for personal use only; you cannot redistribute it and/or use it in 
// your own program.


/** 
 *  \file Logger.cpp
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief  The %Logger class implementation file.
**/

#include <iostream>
#include <string>
#include <map>

#include "LogChannel.h"
#include "Logger.h"
#include "Log.h"
#include "version.h"
#include "os/thread/Thread.h"
#include "os/thread/MutexThread.h"
#include "comms/Message.h"
#include "comms/Channel.h"
#include "comms/Queue.h"

#include <iostream>

using namespace fndts::alf;

/* -- Static member initialization ------------------------------------------ */
Logger *                          Logger::singleton = NULL;
unsigned int                      Logger::glevel    = 0;
bool                              Logger::eflag     = true;
bool                              Logger::wflag     = true;
fndts::os::MutexThread            Logger::mutex;
std::map<std::string,LogChannel*> Logger::channels;
/* ioport is initialized in getLogger as it uses new operator calling Queue
 * constructor (we use Queue as communications channel). Queue needs to do 
 * init stuff (create static objects) before calling the constructor.
 */
fndts::comms::Channel *           Logger::ioport = NULL;

/* -- Object methods -------------------------------------------------------- */

// Public object method: logChannelExists
// Check if a log channel exists
const bool Logger::logChannelExists(const std::string n) const
{
    Logger::mutex.lock();
    bool r = Logger::channels.find(n) != Logger::channels.end();
    Logger::mutex.unlock();
    return r;
}

// Public object method: openLogChannel
// Create a new log channel or obtain a reference to a current  existing one.
LogChannel & Logger::openLogChannel(const std::string n, const unsigned int l)
{
    /* Search if any log channels have the same name */
    if (logChannelExists(n))
    {
        Logger::mutex.lock();
        LogChannel *plc = Logger::channels[n];
        Logger::mutex.unlock();
        return *plc;
    }

    /* Create the new log channel and add it to the channels vector */
    Logger::mutex.lock();
    Logger::channels[n] = new LogChannel(n,l);
    LogChannel & lc = *Logger::channels[n];
    Logger::mutex.unlock();
    return lc;
}

// Public object method: closeLogChannel
// Destroys a LogChannel removing it from the channels vector.
const bool Logger::closeLogChannel(const std::string n)
{
    if ( logChannelExists(n) )
    {
        Logger::mutex.lock();
        LogChannel *plc = Logger::channels[n];
        Logger::channels.erase(n);
        delete plc;
        Logger::mutex.unlock();
        return true;
    }
    return false;
}

// Protected method: threadStartRoutine
// Starts the logger thread (inherited)
void * Logger::threadStartRoutine (void *arg)
{
    bool finish = false;
    while (!finish)
    {
        comms::Message msg;
        if (Logger::ioport->receive(msg))
        {
            /**
             * \todo    Decide action to take when receive msg action fails:
             *          finish normally, exception, ignore,... ?
            **/
            Log log(msg);
            switch (log.getType())
            {
                case eEXIT:     { exit(log); finish=true; break; }
                case eERROR:    { error(log); break;             }
                case eWARNING:  { warning(log); break;           }
                case eSTANDARD: { standard(log); break;          }
                default:
                {
                    std::cerr << "Logger thread: received incorrect message:\n";
                    std::cerr << "  channel: " << log.getChannel() << "\n"
                              << "  thread : " << log.getThreadName() << "\n"
                              << "  level  : " << log.getLevel() << "\n"
                              << "  type   : " << log.getType() << "\n"
                              << "  text   : " << log.getLogText() << "\n";
                    std::cerr.flush();
                }
            }
        }
    }
}

// Private object method: exit
// Manages the exit command
void Logger::exit(const Log & log) const
{
    std::clog << "[" << log.getChannel() << "] (" <<
                 log.getLevel() <<")\t: " << log.getLogText(); 
    std::clog.flush();
}

// Private object method: error
// Manages error messages
void Logger::error(const Log & error) const
{
    if (Logger::eflag) 
    {
        std::cerr << "ERROR ";
        report(std::cerr,error);    
    }
}

// Private object method: warning
// Manages warning messages
void Logger::warning(const Log & warning) const
{
    if (Logger::wflag) 
    {
        std::cerr << "WARNING ";
        report(std::cerr,warning);
    }
}

// Private object method: standard
// Manages standard messages
void Logger::standard(const Log & standard) const
{
    if (standard.getLevel() >= Logger::glevel ) report(std::clog,standard);
}

// Private object method: report
// Outputs a log to the stream
void Logger::report(std::ostream & out, const Log & l) const
{
    out << "[" << l.getChannel() << "]["
        << l.getThreadName() << "][" << l.getLevel() << "]\t: "
        << l.getLogText() << "\n";
    out.flush();
}

/* -- Class methods --------------------------------------------------------- */

// Public class method: getLogger
// Creates the Logger object or returns a reference to the current existing one.
// Implements the singleton pattern.
Logger & Logger::getLogger(unsigned int l)
{
    Logger::mutex.lock();
    if (!Logger::singleton)
    {
        /* Static attribute initialized here as it uses new operator */
        /* We use a Queue as the communications port */
        Logger::ioport = new fndts::comms::Queue();

        /* Getting the logger object */
        Logger::singleton = new Logger(l);
    }
    Logger::mutex.unlock();
    return * Logger::singleton;
}

// Public class method: getVersion
// Returns the version of the ALF library.
const char * Logger::getVersion()
{
    return ALF_VERSION;
}

// Public class method: getGlobalLevel
// Returns the global log level
unsigned int Logger::getGlobalLogLevel()
{
    return Logger::glevel;
}

// Public class method: setGlobalLevel
// Sets a new global log level
void Logger::setGlobalLogLevel(unsigned int l)
{
    Logger::mutex.lock(); 
    Logger::glevel = l; 
    Logger::mutex.unlock();
}

// Public class method: getErrorFlag
// Returns the error flag
const bool Logger::getErrorFlag()
{
    return Logger::eflag;
}

// Public class method: setErrorFlag
// Sets the error flag
void Logger::setErrorFlag(const bool e)
{
    Logger::mutex.lock(); 
    Logger::eflag = e;
    Logger::mutex.unlock(); 
}

// Public class method: getWarningFlag
// Returns the warning flag
const bool Logger::getWarningFlag()
{
    return Logger::wflag;
}

// Public class method: setWarningFlag
// Sets the warning flag
void Logger::setWarningFlag(const bool w)
{
    Logger::mutex.lock(); 
    Logger::wflag=w;
    Logger::mutex.unlock(); 
}

// Public class method: debug
// Ouputs in standard output debug information.
void Logger::debug()
{
    using namespace std;

    Logger::mutex.lock();
    cout << "dbg> ALF4C++ debug" << endl;
    if (!Logger::singleton) cout <<"dbg> Logger object does not exist."<< endl;
    cout << "dbg> Global log level = " << Logger::getGlobalLogLevel() << endl;
    cout << "dbg> Active log channels:" << endl;
    std::map<std::string,LogChannel*>::const_iterator ite;
    for(ite=Logger::channels.begin(); ite!=Logger::channels.end(); ite++)
    {
        cout << "dbg> \t" << (*ite).second->getName() << " with level " 
             << (*ite).second->getLogLevel() << endl;
    }
    cout << "dbg> ALF4C++ debug end" << endl;
    Logger::mutex.unlock();
}

// Public class method: close
// Close the logger destroying it
const bool Logger::close()
{
    Logger::mutex.lock();
    if (Logger::singleton)
    {
        /* Send termination message to the logger's thread */
        Log exit(eEXIT,0,"Logger destructor","Destroying Logger object upon "
                 "close request. No more logging facilities available to the "
                 "program.\n");
        Logger::ioport->send(exit.toMessage());

        /* Wait for the thread to finish */
        Logger::mutex.unlock();
        getLogger().join();

        /* Now delete the instance (call Logger destructor) and done */
        Logger::mutex.lock();
        delete Logger::singleton;
        Logger::mutex.unlock();
        return true;
    }
    Logger::mutex.unlock();
    return false;
}

/* -- Constructors ---------------------------------------------------------- */

// Private constructor: Logger
// Constructor by global level. It is private as the Logger object must be
// created using a call to getLogger (this one is the one which will do the 
// new calling the constructor, the user cannot perform "new Logger" operation).
Logger::Logger(unsigned int l)
:
    /* Superclass construction */
    Thread("Logger")
{
    /* Configure global log level */
    Logger::glevel = l;

    /* Start the execution of the thread */
    launch(NULL);
}

/* -- Destructor ------------------------------------------------------------ */

// Private destrcutor: ~Logger
// This method is called from the close method.
// Destroys all the associated log channels, empties the channels vector and
// leaves the Logger class ready to allow a new instance creation.
// The superclass (Thread) destructor will destroy the thread.
Logger::~Logger()
{
    if (Logger::singleton)
    {
        /* Close and destroy the channel */
        Logger::ioport->close();
        delete dynamic_cast<comms::Queue*>(Logger::ioport);

        /* Destoy all log channels */
        std::map<std::string,LogChannel*>::iterator ite;
        for(ite=Logger::channels.begin(); ite!=Logger::channels.end(); ite++)
        {
            LogChannel *plc = (*ite).second;
            delete plc;
        }

        /* Reset static members for next singleton instantiation */
        Logger::channels.clear();
        Logger::singleton = NULL;
        Logger::ioport = NULL;
        Logger::glevel = 0;
    }
}

/* -- Operators related with Logger class ----------------------------------- */

