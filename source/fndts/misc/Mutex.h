// Foundations library (fndts): Mutex class definintion -*- C++ -*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file is part of the Dynasteis game. This library is intended for 
// personal use only; you cannot redistribute it and/or use it in your own 
// program.

/**
 *  \file   Mutex.h
 *  \author Victor Garcia <vichor@gmail.com>
 *  \brief  The %Mutex class header file.
**/

/* Avoid multiple inclusions */
#pragma once

/* Include files */

/* Namespace definition and forward declarations */
namespace fndts { class Mutex; }

/**
 *  \ingroup fndts
 *  \brief   Abstract class defining mutex behavior. 
**/
class fndts::Mutex 
{
    public:
        /**
         *  \brief %Mutex destruction.
        **/
        virtual ~Mutex();

        /**
         *  \brief  Locks this mutex object.
         *  \return 0 on success; otherwise, the error code.
        **/
        virtual int lock() = 0;

        /**
         *  \brief  Unlocks this mutex object.
         *  \return 0 on success; otherwise, the error code.
        **/
        virtual int unlock() = 0;
};

