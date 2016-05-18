// Foundations library (fndts): SysQueue class definintion -*- C++-*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file was developed as part of the Dynasties game. This library is intended for
// personal use only; you cannot redistribute it and/or use it in your own program.

/**
 *  \file   SysQueue.h
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief  The %SysQueue class header file.
**/

/* Avoid multiple inclusions */
#pragma once

/* Include files */
#include <sys/types.h> 
#include "Channel.h"
#include "Message.h"
#include "SysQueueMessage.h"

/* Namespace definition and forward declarations */
namespace fndts { namespace comms { class SysQueue; } }

/**
 *  \ingroup comms
 *  \brief   A message queue to communicate two Node objects in the same
 *           computer.
 *  \todo    Define SysQueue class.
**/
class fndts::comms::SysQueue : public fndts::comms::Channel
{
    private:
        static const char *keyName;     /* File for ftok() call */
        static const char projectID;    /* Project ID for ftok() call */
        static const int permissions;   /* SysQueue permissions */
        key_t key;  /* Message SysQueue system key for creation */
        int id;     /* Message SysQueue ID */
        bool master;    /* Indicates if this instance is the master of the q. */

    public:
        /**
         *  \brief  Creates a system message queue.
        **/
        SysQueue();

        /**
         *  \brief  Creates a new %SysQueue and attaches it to the given system
         *          queue.
         *  \param  qid The system id of the queue to attach to.
        **/
        explicit SysQueue(int qid);

        /**
         *  \brief  Destoys a system message queue.
        **/
        ~SysQueue();

        /**
         *  \brief  Gets the System identification of this queue.
         *  \return The ID of this queue.
        **/
        inline const int getSystemID() const
        { return id; }

        /**
         *  \brief  Closes the %queue cancelling all pending %messages.
        **/
        virtual const bool close();

        /**
         *  \brief  Sends a %message to this queue.
         *  \param  m   %Message to send.
         *  \return true if all OK; false, otherwise.
        **/
        virtual const bool send(const fndts::comms::Message & m) const;

        /**
         *  \brief  Sends a %message to this queue.
         *  \param  m   %Message to send.
         *  \return true if all OK; false, otherwise.
        **/
        virtual const bool send(const fndts::comms::SysQueueMessage & m) const;

        /**
         *  \brief  Receives a %message from this queue.
         *  \param  r   The received message will be written here.
         *  \return true if everything ok; false, otherwise
        **/
        virtual const bool receive (fndts::comms::Message & r);

        /**
         *  \brief  Receives a %message of the given type from this queue.
         *  \param  r   The received message will be written here.
         *  \return true if everything ok; false, otherwise
        **/
        virtual const bool receive (fndts::comms::SysQueueMessage & r);

};

