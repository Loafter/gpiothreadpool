//
//  GpioI2cIO.h
//  ThreadPool
//
//  Created by Andrew Rose on 14.08.13.
//  Copyright (c) 2013 Andrew Rose. All rights reserved.
//

#ifndef __ThreadPool__GpioI2cIO__
#define __ThreadPool__GpioI2cIO__

#include <iostream>
#include <thread>
#include <condition_variable>
#include "BCM2835Initer.h"
#include <vector>
class GpioI2cIO
{
private:
    BCM2835Initer bCM2835Initer;
    std::mutex pinLock;
public:
    std::vector<char> ReadI2cAd(int portId,int byteCount);
    void WriteI2cAd(int portId,std::vector<char> data);
    
};
#endif /* defined(__ThreadPool__GpioI2cIO__) */
