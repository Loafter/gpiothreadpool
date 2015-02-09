//
//  BCM2835Initer.h
//  ThreadPool
//
//  Created by Andrew Rose on 07.08.13.
//  Copyright (c) 2013 Andrew Rose. All rights reserved.
//

#ifndef __ThreadPool__BCM2835Initer__
#define __ThreadPool__BCM2835Initer__

#include <iostream>
class BCM2835Initer
{
    static int objectCount;
public: 
    BCM2835Initer();
    ~BCM2835Initer();
};
#endif /* defined(__ThreadPool__BCM2835Initer__) */
