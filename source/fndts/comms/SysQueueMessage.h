// Foundations library (fndts): SysQueueMessage class definintion -*- C++-*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file was developed as part of the Dynasties game. This library is intended for
// personal use only; you cannot redistribute it and/or use it in your own program.

/**
 *  \file   SysQueueMessage.h
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief  The %SysQueueMessage class header file.
**/

/* Avoid multiple inclusions */
#pragma once

/* Include files */
#include "Message.h"

/* Namespace definition and forward declarations */
namespace fndts { namespace comms { class SysQueueMessage; } }

/**
 *  \ingroup comms
 *  \brief   A Message to be sent/received through a SysQueue.
 *
**/
class fndts::comms::SysQueueMessage : public fndts::comms::Message
{
    private:
        long messagetype;    /* Identifier of the type of message */

    public:

        /**@{**/
        /**
         *  \brief  Creates a new Message to be sent/received through a Queue.
         *  \param  type The message type identifier in queue context.
         *  \param  sz   The size of the array of data to set in the message.
         *  \param  msg  The array of data.
        **/
        SysQueueMessage();
        SysQueueMessage(const long type, const size_t sz, const tByte *array);
        explicit SysQueueMessage(const long type);
        /**@}**/

        /**
         *  \brief  Destroys the message.
        **/
        virtual ~SysQueueMessage();

        /**
         *  \brief  Gets the type of message in queue context.
         *  \return The type of the message.
        **/
        inline const long getType() const
        { return messagetype; }

        /**
         *  \brief  Sets the type of message to this %SysQueueMessage.
         *  \param  type    The new type of message.
        **/
        inline void setType(const long type)
        { messagetype = type; }
};
