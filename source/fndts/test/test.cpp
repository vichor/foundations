// Foundations library: test application focusing on ALF -*- C++ -*-

// Copyright (C) 2009
// Victor Garcia Santos
//
// This file was developed as part of the Dynasties game. This library is 
// intended for personal use only; you cannot redistribute it and/or use it in 
// your own program.

#include <iostream>
#include <string>
#include "alf/LogChannel.h"
#include "alf/Logger.h"
#include "os/thread/Thread.h"

using namespace fndts;

/* Tests the ALF library.  */
void alftest() 
{
    alf::Logger & l1 = alf::Logger::getLogger(5);
    alf::LogChannel & lc1 = l1.openLogChannel("canal1");
    alf::LogChannel & lc2 = l1.openLogChannel("canal2",50);

    lc1.log(0,"Log with level 0 and log channel level 0");   // logger filters
    lc2.log(0,"Log with level 0 and log channel level 50");  // logch. filters
    lc2.log(70,"Log with level 70 and log channel level 50");// issued
    lc1.log(30,"Log with level 30 and log channel level 0"); // issued
    lc2.log(30,"Log with level 30 and log channel level 50");// logch. filters

    lc1.warning("This is a warning!!");
    lc2.error("This is an error!!");

    l1.setGlobalLogLevel(0);
    lc1 << "A log created with the operator << " 
        << "including two insertions. New line at the end of the 2nd one.\n"
        << "3rd and 4th insertions are done in the same command including "
        << "other data types: " << 23 << " " << 4.3 << "\n";

    /* 
     * This cannot be done as we will invalidate references of other logs:
     *      Thread 1 starts and runs to just before calling closeLogChannel.
     *      Thread 2 starts and takes CPU from Thread 1 running to lc2.log
     *      Thread 1 takes CPU and closes log channels
     *      Thread 2 takes CPU and tries to log on channel 2 which is already
     *          closed. => Seg. fault.
     */
    //l1.closeLogChannel(lc2.getName());
    //l1.closeLogChannel(lc1.getName());
}

/* My thread class to test ALf with concurrent accesses */
class OneThread : public os::Thread
{
    private:
    OneThread() : Thread("disabled constructor") {}
    OneThread & operator=(const OneThread &src) {}

    protected:
    void * threadStartRoutine(void *arg)
    { alftest(); }

    public:
    OneThread(const char *n) : Thread(n) {}
    ~OneThread() {}
};

/* Main function */
int main()
{
    OneThread t1("thread 1");
    OneThread t2("thread 2");
    OneThread t3("thread 3");
    OneThread t4("thread 4");
    OneThread t5("thread 5");
    OneThread t6("thread 6");
    OneThread t7("thread 7");
    OneThread t8("thread 8");
    OneThread t9("thread 9");
    OneThread ta("thread a");
    t1.launch(NULL);
    t2.launch(NULL);
    t3.launch(NULL);
    t4.launch(NULL);
    t5.launch(NULL);
    t6.launch(NULL);
    t7.launch(NULL);
    t8.launch(NULL);
    t9.launch(NULL);
    ta.launch(NULL);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    t8.join();
    t9.join();
    ta.join();
    alf::Logger::getLogger().close();
    return 0;
}
