// Foundations library (fndts): Queue class definintion -*- C++-*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file was developed as part of the Dynasties game. This library is intended for
// personal use only; you cannot redistribute it and/or use it in your own program.

/**
 *  \file   Queue.h
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief  The %Queue class header file.
**/

/* Avoid multiple inclusions */
#pragma once

/* Include files */
#include "Channel.h"
#include "Message.h"
#include "os/thread/CondThread.h"
#include <queue>
#include <map>

/* Namespace definition and forward declarations */
namespace fndts { namespace comms { class Queue; } }

/**
 *  \ingroup comms
 *  \brief   A message channel to communicate two Thread objects in the same
 *           execution environment using a FIFO queue of Message objects.
 *  \todo    Define Queue class.
**/
class fndts::comms::Queue : public fndts::comms::Channel
{
    private:
        static std::map<unsigned int,Queue*> qlist; /* All existing queues */
        static fndts::os::MutexThread gmutex;   /* Mutex for static members */

        std::queue<Message> q;      /* The fifo queue to store the messages */
        int id;                     /* Queue identifier */
        fndts::os::CondThread msgavail; /* Message available signal */
        fndts::os::MutexThread mutex;   /* Mutex for object members */

        /* Copy constructor and assignment operator disabled */
        Queue(const Queue & src):Channel("disabled") {}
        Queue(Queue & src):Channel("disabled") {}
        Queue & operator = (const Queue & src) {}

    public:
        /**
         *  \brief  Creates a queue.
        **/
        Queue();

        /**
         *  \brief  Destoys a queue.
        **/
        virtual ~Queue();

        /**
         *  \brief  Checks if the %queue with the given ID exists.
         *  \param  n   ID to check.
        **/
        static const bool exists(unsigned int n);

        /**
         *  \brief  Closes the messenger cancelling all pending messages.
        **/
        virtual const bool close();

        /**
         *  \brief  Sends a Message to this messenger.
         *  \param  m   Message to send.
         *  \return true if all OK; false, otherwise.
        **/
        virtual const bool send(const comms::Message & m);

        /**
         *  \brief  Receives a Message from this messenger.
         *  \param  r   The received message will be written here.
         *  \return true if everything ok; false, otherwise
        **/
        virtual const bool receive (comms::Message & r);

        /**
         *  \brief  Gets the identifier of this Queue.
         *  \return The Id
        **/
        inline unsigned int getID()
        { return id; }

        /**
         *  \brief  Gets the given %Queue.
         *  \param  n   The ID of the Queue to get.
         *  \return The asked %Queue.
        **/
        static Queue * getQueue(unsigned int n);
};

