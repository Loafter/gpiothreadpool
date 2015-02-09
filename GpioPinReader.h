//
//  GpioPinReader.h
//  ThreadPool
//
//  Created by Andrew Rose on 13.08.13.
//  Copyright (c) 2013 Andrew Rose. All rights reserved.
//

#ifndef __ThreadPool__GpioPinReader__
#define __ThreadPool__GpioPinReader__

#include <iostream>
#include "BCM2835Initer.h"
class GpioPinReader
{
    const int pin;
    BCM2835Initer BCM2835Init;
public:
    GpioPinReader(int pin_):pin(pin_){};
    int GetPinState();
    
};
#endif /* defined(__ThreadPool__GpioPinReader__) */
