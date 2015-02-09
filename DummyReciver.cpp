//
//  DummyReciver.cpp
//  ThreadPool
//
//  Created by Andrew Rose on 24.07.13.
//  Copyright (c) 2013 Andrew Rose. All rights reserved.
//
#include <functional>
#include <algorithm>

#include "DummyReciver.h"

using namespace std;

DummyReciver::~DummyReciver()
{
    if(!newSignalCondition)delete newSignalCondition;
    if(!queLock)delete queLock;
    
}
void DummyReciver::processSignal(const string& Signal)
{
    std::chrono::milliseconds dura(2);
    std::this_thread::sleep_for(dura);
    std::cout<<Signal<<std::endl;
}

const bool DummyReciver::waitPred()
{
    return interuptFlag|newsignalFlag;
}

void DummyReciver::DoJob(DummyReciver* thisDummyReciver)
{
 while (true)
 {
     vector<string> buffer;
     {       
         unique_lock<mutex> lck(*thisDummyReciver->queLock);
         thisDummyReciver->newSignalCondition->wait(lck,bind(mem_fun(&DummyReciver::waitPred), thisDummyReciver));
         if(thisDummyReciver->interuptFlag)
             return;
         while (!thisDummyReciver->signalQueue.empty())
         {
             for(int i=0;i<thisDummyReciver->signalQueue.size();++i)
             {
                 buffer.push_back(thisDummyReciver->signalQueue.front());
                 thisDummyReciver->signalQueue.pop();
             }
         }
         thisDummyReciver->newsignalFlag=false;
     }
     for_each(buffer.begin(),buffer.end(), [=] (vector<string>::value_type element)
              {
                  thisDummyReciver->processSignal(element);
              });              
 }
}

void DummyReciver::TerminateThread()
{
    {
        unique_lock<mutex> lck(*queLock);
        interuptFlag=true;
        newSignalCondition->notify_all();
    }
    
}

void DummyReciver::ReciveSignal(const string& Signal)
{
    {
        unique_lock<mutex> lck(*queLock);
        signalQueue.push(Signal);
        newsignalFlag=true;
        newSignalCondition->notify_all();
    }
}
