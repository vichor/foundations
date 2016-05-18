// Foundations library (alf-another logging facility): LogChannel -*- C++ -*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file was developed as part of the Dynasties game. This library is 
// intended for personal use only; you cannot redistribute it and/or use it in 
// your own program.


/** 
 *  \file LogChannel.cpp
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief The %LogChannel class implementation.
**/

#include "LogChannel.h"
#include "Logger.h"
#include "Log.h"
#include "comms/Queue.h"
#include "comms/Message.h"
#include "os/thread/MutexThread.h"
#include <string>

using namespace fndts::alf;

/* -- Static member initialization ------------------------------------------ */

/* -- Object methods -------------------------------------------------------  */

// Public object method: log
// Outputs a log with the default log level.
const bool LogChannel::log(const std::string & log)
{
    return this->log(deflev,log);
}

// Public object method: log
// Outputs a log to the Logger object but only in case that the given level is 
// equal or greater than the LogChannel's one and the Logger's global level.
// In case the log was issued, it returns true; false, otherwise.
const bool LogChannel::log(const unsigned int l, const std::string & log)
{
    bool sent=false;
    this->mutex.lock();
    if (this->level <= l)
    { 
        Log newlog(eSTANDARD,l,getName(),log);
        Logger::getIOPort().send(newlog.toMessage());
        sent=true; 
    }
    this->mutex.unlock();
    return sent;
}

// Public object method: error
// Issues an error if the flag is set.
const bool LogChannel::error(const std::string & e)
{
    bool sent=false;
    this->mutex.lock();
    if (this->eflag)
    {
        Log msg(eERROR,Logger::getGlobalLogLevel(),getName(),e);
        Logger::getIOPort().send(msg.toMessage());
        sent=true; 
    }
    this->mutex.unlock();
    return sent;
}

// Public object method: warning
// Issues a warning if the flag is set.
const bool LogChannel::warning(const std::string & w)
{
    bool sent=false;
    this->mutex.lock();
    if (this->eflag)
    {
        Log msg(eWARNING,Logger::getGlobalLogLevel(),getName(),w);
        Logger::getIOPort().send(msg.toMessage());
        sent=true; 
    }
    this->mutex.unlock();
    return sent;
}

// Private method: doInsertInStreamOp
// This is the actual implementation for the template operator <<. The operator
// cannot be placed in this file because it is a template, and must have the
// implementation available for their users. The operator, thus, will just call
// this function after having updated the logstream attribute.
void LogChannel::doInsertInStreamOp()
{
    /* Get the string from the stream */
    std::string logbuf = this->logstream.str();
            
    /* Search for all new lines */
    int newlinepos = logbuf.find('\n');
    while (newlinepos != std::string::npos)
    {
        /* Get the substring from beggining to new line and send it */
        std::string newlog = logbuf.substr(0,newlinepos);
        this->mutex.unlock();
        log(getDefaultLogLevel(),newlog); 
        this->mutex.lock();

        /* Cut the already sent substring */
        logbuf = logbuf.substr(newlinepos+1);

        /* Next new line character */
        newlinepos = logbuf.find('\n');
    }

    /* Update the stream and out */
    this->logstream.str(logbuf);
}


/* -- Constructors ---------------------------------------------------------- */

// Private constructor: LogChannel
// Constructs a new log channel with the given name and log level. The default
// log level is initialized to the channel's log level to ensure that logs
// issued with the << operator arise. The user may change this calling the
// method setDefaultLogLevel.
// The Logger object must be initialized before creating a LogChannel. It it is
// not, the getLogger call will create it, but with default values.
LogChannel::LogChannel(std::string n, const unsigned int l) 
: 
    /* Attributes construction */
    level(l), 
    deflev(l), 
    name(n),
    wflag(true),
    eflag(true),
    mutex(),
    logstream()
{
}

/* -- Destructor ------------------------------------------------------------ */

// Private destructor: ~LogChannel
// Destroys the LogChannel.
LogChannel::~LogChannel()
{
}

/* -- Operators related with LogChannel class ------------------------------- */

