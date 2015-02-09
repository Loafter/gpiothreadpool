//
//  BasicThread.h
//  ThreadPool
//
//  Created by Andrew Rose on 22.07.13.
//  Copyright (c) 2013 Andrew Rose. All rights reserved.
//

#ifndef __ThreadPool__BasicThread__
#define __ThreadPool__BasicThread__

#include <iostream>
#include <thread>
#include <queue>
#include <condition_variable>
template<typename I>
    class IBasicThreads
    {
    private:
        std::thread workThread;
        std::condition_variable newSignalCondition;
        I& workFunctor;
    public:
        IBasicThreads(I& func);
        void ReciveSignal(const typename I::value_type& signal);
        ~IBasicThreads();
    };

template<typename I>
IBasicThreads<I>::IBasicThreads(I& func):workFunctor(func),workThread(I::DoJob,&func)
    {
    
        
    }

template<typename I>
IBasicThreads<I>::~IBasicThreads()
    {
        workFunctor.TerminateThread();
        workThread.join();
    }
 
template<typename I>
void IBasicThreads<I>::ReciveSignal(const typename I::value_type& signal)
    {
        workFunctor.ReciveSignal(signal);
    }
    
#endif /* defined(__ThreadPool__BasicThread__) */
