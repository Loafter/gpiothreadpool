//
//  DummyReciver.h
//  ThreadPool
//
//  Created by Andrew Rose on 24.07.13.
//  Copyright (c) 2013 Andrew Rose. All rights reserved.
//

#ifndef __ThreadPool__DummyReciver__
#define __ThreadPool__DummyReciver__

#include <iostream>
#include <thread>
#include <queue>
#include <condition_variable>

class DummyReciver//:counter
{
private:
    std::queue<std::string> signalQueue;
    std::mutex* queLock;
    std::condition_variable* newSignalCondition;
    void processSignal(const std::string& Signal);
    const bool waitPred();
    bool interuptFlag;
    bool newsignalFlag;
public:
    typedef std::string  value_type;
    DummyReciver():newsignalFlag(false),interuptFlag(false),queLock(new std::mutex),
                  newSignalCondition(new std::condition_variable)
    {
  
    }
    ~DummyReciver();
    static void DoJob(DummyReciver* thisDummyReciver);
    void TerminateThread();
    void ReciveSignal(const std::string& Signal);
};
#endif /* defined(__ThreadPool__DummyReciver__) */
