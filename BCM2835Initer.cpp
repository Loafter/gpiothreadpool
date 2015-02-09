//
//  BCM2835Initer.cpp
//  ThreadPool
//
//  Created by Andrew Rose on 07.08.13.
//  Copyright (c) 2013 Andrew Rose. All rights reserved.
//

#include <bcm2835.h>
#include "BCM2835Initer.h"
int BCM2835Initer::objectCount=0;
BCM2835Initer::BCM2835Initer()
{
    if(objectCount==0)
    {
        if (!bcm2835_init())
         throw "error init bcm2835";
    ++objectCount;
    }
}
BCM2835Initer::~BCM2835Initer()
{
    --objectCount;
    if(objectCount==0)
        bcm2835_close();
}
