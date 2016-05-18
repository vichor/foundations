// Foundations library (comms): Channel class definintion -*- C++-*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file was developed as part of the Dynasties game. This library is intended for
// personal use only; you cannot redistribute it and/or use it in your own program.

/**
 *  \file   Channel.h
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief  The %Channel class header file.
**/

/* Avoid multiple inclusions */
#pragma once

/* Include files */
#include <string>

/* Namespace definition and forward declarations */
namespace fndts { namespace comms { 
    class Channel; 
    class Message;
} }

/**
 *  \ingroup comms
 *  \brief   A communication channel to send and/or receive Message objects.
**/
class fndts::comms::Channel 
{
    private:
        std::string name;

        /* Copy constructor disabled. */
        Channel(const Channel & src);

    protected:

        /**
         *  \brief  Constructs a %Channel by setting the name to it.
         *  \param  n   The new name.
        **/
        Channel(const std::string n);
        Channel(const char *n);

        /**
         *  \brief  Destroys a channel.
        **/
        virtual ~Channel();

    public:

        /**
         *  \brief  Gets the name of this channel.
         *  \return The name of the channel.
        **/
        inline const std::string getName() const
        { return name; }

        /**
         *  \brief  Closes this channel cancelling all pending communications.
        **/
        virtual const bool close() = 0;

        /**
         *  \brief  Sends a Message through this channel.
         *  \param  m   Message to send.
         *  \return true if the Message was sent successfully; false, otherwise.
        **/
        virtual const bool send(const fndts::comms::Message & m) = 0;

        /**
         *  \brief  Gets a Message from this channel.
         *  \param  r   The Message received will be loaded here.
         *  \return true if all ok; false, otherwise
        **/
        virtual const bool receive (fndts::comms::Message & r) = 0;
};

