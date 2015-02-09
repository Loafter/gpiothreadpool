//
//  main.cpp
//  ThreadPool
//
//  Created by Andrew Rose on 22.07.13.
//  Copyright (c) 2013 Andrew Rose. All rights reserved.
//


#include <iostream>

#include <stdio.h>
#include <string.h>
#include <sstream>
#include "GPIOWebService.h"
using namespace std;

int main(int argc, const char * argv[])
{  
    GPIOWebService gPIOWebService;
    gPIOWebService.StartListen(8888);
    gPIOWebService.WaitForTerminate();
    return 0;
}

