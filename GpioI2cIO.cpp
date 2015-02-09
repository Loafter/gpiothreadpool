//
//  GpioI2cIO.cpp
//  ThreadPool
//
//  Created by Andrew Rose on 14.08.13.
//  Copyright (c) 2013 Andrew Rose. All rights reserved.
//

#include "GpioI2cIO.h"
#include "BCM2835Initer.h"
#include <bcm2835.h>
#include <thread>
#include <condition_variable>

using namespace std;
vector<char> GpioI2cIO::ReadI2cAd(int portId,int byteCount)
{
    vector<char> buffer(byteCount);
    {unique_lock<mutex> lck(pinLock);
        bcm2835_i2c_begin();
        bcm2835_i2c_setSlaveAddress(portId);        
        bcm2835_i2c_read(&buffer.front(),byteCount);
        bcm2835_i2c_end();
    }
    return buffer;
}
void GpioI2cIO::WriteI2cAd(int portId,vector<char> data)
{
    {unique_lock<mutex> lck(pinLock);
        bcm2835_i2c_begin();
        bcm2835_i2c_setSlaveAddress(portId);
        bcm2835_i2c_write(&data.front(),data.size());
        bcm2835_i2c_end();
    }
}
