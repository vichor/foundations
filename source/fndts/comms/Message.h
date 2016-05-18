// Foundations library (fndts): Message class definintion -*- C++-*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file was developed as part of the Dynasties game. This library is intended for
// personal use only; you cannot redistribute it and/or use it in your own program.

/**
 *  \file   Message.h
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief  The %Message class header file.
**/

/* Avoid multiple inclusions */
#pragma once

/* Include files */

/* Namespace definition and forward declarations */
namespace fndts { namespace comms {
    class Message; 
    typedef unsigned char tByte; 
} }

/**
 *  \ingroup comms
 *  \brief   A message to be sent/received through a Channel.
**/
class fndts::comms::Message 
{
    protected:
        tByte   *data;  /* The array where the data are sent from/received to */
        size_t  msgsize;    /* The size of the array */

    public:
        /**@{**/
        /**
         *  \brief  Creates a new message with the given data.
         *  \param  sz      Size of the given array with the initial data.
         *  \param  array   The array with the initial data.
        **/
        Message();
        Message(const size_t sz, const tByte *array);
        /**@}**/

        /**
         *  \brief  Copy constructor
         *  \param  src The source %Message to copy from.
        **/
        Message(const Message & src);
        Message(Message & src);

        /**
         *  \brief  Deallocates a %Message.
        **/
        virtual ~Message();

        /**
         *  \brief  Gets the size in bytes of the user data.
         *  \return The size in bytes of the user data.
        **/
        virtual const size_t size() const;

        /**
         *  \brief  Gets an array containing the message data.
         *
         *  The array must be allocated previously by the user. Use the method
         *  size() to get the amount of needed bytes.
         *
         *  \param  array   An array where the message data will be copied to.
        **/
        virtual void toByteArray(tByte *array) const;

        /**
         *  \brief  Loads the given bytes in the object.
         *
         *  Calling this method will delete all previously loaded data.
         *
         *  \param sz       Size in bytes of the given array of data.
         *  \param array    Array of bytes to load.
        **/
        virtual void fromByteArray(const size_t sz, const tByte *array);

        /**
         *  \brief  Copies the given %Message to the current one.
         *  \param  src The %Message to copy from.
        **/
        virtual Message & operator = (const Message & src);
};

