//
//  GpioReciver.h
//  ThreadPool
//
//  Created by Andrew Rose on 05.08.13.
//  Copyright (c) 2013 Andrew Rose. All rights reserved.
//

#ifndef __ThreadPool__GpioReciver__
#define __ThreadPool__GpioReciver__

#include <iostream>
#include <thread>
#include <queue>
#include <condition_variable>
#include "GpioSignals.h"
#include "BCM2835Initer.h"
#include <memory>
class GpioReciver//:counter
{
private:
    BCM2835Initer BCM2835Init;
    std::queue<std::shared_ptr<BasicGpioSignal> > signalQueue;
    std::mutex* queLock;
    std::condition_variable* newSignalCondition;
    void SetHiLevel(const SetHiLevelSignal& Signal);
    void SetLowLevel(const SetLowLevelSignal& Signal);
    void processSignal(const std::shared_ptr<BasicGpioSignal>& Signal);

    const bool waitPred();
    bool interuptFlag;
    bool newsignalFlag;
    int pin;
public:
    typedef std::shared_ptr<BasicGpioSignal>  value_type;
    GpioReciver(uint8_t pin_):newsignalFlag(false),interuptFlag(false),queLock(new std::mutex),
    newSignalCondition(new std::condition_variable),pin(pin_)
    {
        
    }
    ~GpioReciver();
    static void DoJob(GpioReciver* thisDummyReciver);
    void TerminateThread();
    void ReciveSignal(const std::shared_ptr<BasicGpioSignal>& Signal);
};


#endif /* defined(__ThreadPool__GpioReciver__) */
