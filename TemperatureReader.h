//
//  TemperatureReader.h
//  ThreadPool
//
//  Created by Andrew Rose on 06.08.13.
//  Copyright (c) 2013 Andrew Rose. All rights reserved.
//

#ifndef __ThreadPool__TemperatureReader__
#define __ThreadPool__TemperatureReader__
#include <string>
#include "BCM2835Initer.h"
class TemperatureReader
{
    BCM2835Initer BCM2835Init;
    const int pinNumber;
    std::string readDHTF(int pin);
    std::string readDHTH(int pin);
public:
    TemperatureReader(const int pinNumber_);
    std::string GetTemperature();
    std::string GetHumidity();
};
#endif /* defined(__ThreadPool__TemperatureReader__) */
