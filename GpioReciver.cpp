//
//  GpioReciver.cpp
//  ThreadPool
//
//  Created by Andrew Rose on 05.08.13.
//  Copyright (c) 2013 Andrew Rose. All rights reserved.
//

#include <functional>
#include <algorithm>
#include <bcm2835.h>
#include "GpioReciver.h"

using namespace std;

GpioReciver::~GpioReciver()
{
    if(!newSignalCondition)delete newSignalCondition;
    if(!queLock)delete queLock;
    
}
void GpioReciver::SetHiLevel(const SetHiLevelSignal& Signal)
{
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(pin, HIGH);
    cout << "Set hi level on "<<pin<<" pin"<<endl;
}
void GpioReciver::SetLowLevel(const SetLowLevelSignal& Signal)
{
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(pin, LOW);
    cout << "Set low level on "<<pin<<" pin"<<endl;

}
void GpioReciver::processSignal(const std::shared_ptr<BasicGpioSignal>& Signal)
{
    try
    {
        if(Signal.get()->SignalType==SignalTypeEnum::SetHiLevelPin)
        {
            SetHiLevel(*static_cast<SetHiLevelSignal*>(Signal.get()));
        }
        else if (Signal.get()->SignalType==SignalTypeEnum::SetLowLevelPin)
        {
            SetLowLevel(*static_cast<SetLowLevelSignal*>(Signal.get()));
        }
        else
            cout << "Wront type signal in GpioReciver"<< endl;
    } catch (exception ex)
    {
        cout << "Standard exception: " << ex.what() << endl;
    }
}

const bool GpioReciver::waitPred()
{
    return interuptFlag|newsignalFlag;
}

void GpioReciver::DoJob(GpioReciver* thisGpioReciver)
{
    while (true)
    {
        vector<std::shared_ptr<BasicGpioSignal> > buffer;
        {
            unique_lock<mutex> lck(*thisGpioReciver->queLock);
            thisGpioReciver->newSignalCondition->wait(lck,bind(mem_fun(&GpioReciver::waitPred), thisGpioReciver));
            if(thisGpioReciver->interuptFlag)
                return;
            while (!thisGpioReciver->signalQueue.empty())
            {
                for(int i=0;i<thisGpioReciver->signalQueue.size();++i)
                {
                    buffer.push_back(thisGpioReciver->signalQueue.front());
                    thisGpioReciver->signalQueue.pop();
                }
            }
            thisGpioReciver->newsignalFlag=false;
        }
        for_each(buffer.begin(),buffer.end(), [=] (vector<std::shared_ptr<BasicGpioSignal> >::value_type element)
                 {
                     thisGpioReciver->processSignal(element);
                 });
    }
}

void GpioReciver::TerminateThread()
{
    {
        unique_lock<mutex> lck(*queLock);
        interuptFlag=true;
        newSignalCondition->notify_all();
    }
    
}

void GpioReciver::ReciveSignal(const std::shared_ptr<BasicGpioSignal>& Signal)
{
    {
        unique_lock<mutex> lck(*queLock);
        signalQueue.push(Signal);
        newsignalFlag=true;
        newSignalCondition->notify_all();
    }
}
