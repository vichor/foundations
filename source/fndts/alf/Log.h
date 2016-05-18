// Foundations library (alf-another logging facility): Log class -*- C++ -*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file was developed as part of the Dynasties game. This library is 
// intended for personal use only; you cannot redistribute it and/or use it in 
// your own program.

/** 
 *  \file Log.h
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief  The ALF Log class header file.
**/

/* Avoid multiple inclusions */
#pragma once

/* Include files */
#include "comms/Message.h"
#include <string>

namespace fndts { namespace alf {
    class Log;

    /**
     *  \brief  The types of Log being sent from LogChannel to Logger.
    **/
    enum eLogType
    {
        eEXIT     = 0,       /**< Finish command (ask the thread to finish). **/
        eERROR    = 1,       /**< An error has been detected. **/
        eWARNING  = 2,       /**< A strange but acceptable behavior detected **/
        eSTANDARD = 3,       /**< A standard log **/
        eNUMLOGTYPES = eSTANDARD + 1
    };

} }


/**
 *  \ingroup alf
 *  \brief  A log sent from the LogChannel to the Logger.
 *
 *  This class is a log object that can be sent trough a Queue to the Logger.
 *  When the LogChannel class is asked to send a %Log, it creates an instance
 *  of this class with the needed information and sends it to the Logger through
 *  a Queue.
 *
 *  The data sent includes:
 *      - The log text.
 *      - The channel name which originated this log.
 *      - The level of the log.
 *      - The type of the log (see eLogType).
 *
 *  As Queue's send() method accepts only Message objects, this class implements
 *  a toMessage() method that transforms it into a Message.
**/
class fndts::alf::Log
{
    private:
        std::string text;       /* Test of the log to output */
        std::string channel;    /* Channel name              */
        std::string thread;     /* Thread name               */
        unsigned int level;     /* Level of the log          */
        eLogType type;          /* Type of log               */

        /* Default constructor disabled */
        Log() 
        { }

        /* 
         * this method is called from both constructors to initialize the
         * members. It is used to not repeat in the constructors the same code
        */
        void initialize(const eLogType k, const unsigned int l, const char *c, 
                        const char *t);

    public:
        /**@{**/
        /**
         *  \brief  Creates a new log entry.
         *  \param  k   The kind(type) of log. See eLogType.
         *  \param  l   The level of the log.
         *  \param  t   Text that will be output (the log itself).
         *  \param  c   Channel that originated the log.
        **/
        Log(const eLogType k, const unsigned int l, const char *c, 
            const char *t);
        Log(const eLogType k, const unsigned int l, const std::string &c, 
            const std::string &t);
        /**@}**/

        /**
         *  \brief  Creates a new %Log from a Message
         *  \param  src Message received through a Queue and containing a %Log.
        **/
        Log(const comms::Message src);

        /**
         *  \brief  Deallocates a %Log.
        **/
        virtual ~Log();

        /**
         *  \brief  Gets the text of this %Log.
         *  \return The log text of this %Log.
        **/
        inline const std::string getLogText() const
        { return text; }

        /**
         *  \brief  Gets the name of the Thread which created this log.
         *  \return The name of the Thread of this %Log.
        **/
        inline const std::string getThreadName() const
        { return thread; }

        /**
         *  \brief  Gets the LogChannel that created this log.
         *  \return The name of the channel of this %Log.
        **/
        inline const std::string getChannel() const
        { return channel; }

        /**
         *  \brief  Gets the level of this log.
         *  \return The level of this log.
        **/
        inline const unsigned int getLevel() const
        { return level; }

        /**
         *  \brief  Gets the type of this log.
         *  \return The type of the log. 
        **/
        inline const eLogType getType() const
        { return type; }

        /**
         *  \brief  Transforms this %Log into a Message to be sent through a
         *          Queue.
         *  \return The representing Message of this %Log.
        **/
        fndts::comms::Message toMessage() const;
};
