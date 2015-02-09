//
//  GPIOWebService.h
//  ThreadPool
//
//  Created by Andrew Rose on 05.08.13.
//  Copyright (c) 2013 Andrew Rose. All rights reserved.
//

#ifndef __ThreadPool__GPIOWebService__
#define __ThreadPool__GPIOWebService__

#include <iostream>
#include <iostream>
#include "BasicThread.h"
#include "DummyReciver.h"
#include "GpioReciver.h"

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include "GpioPinReader.h"
#include "TemperatureReader.h"

#include <thread>
#include <condition_variable>


    


class GPIOWebService
{
private:
    static int reciverSignalMethod(void *cls, struct MHD_Connection *connection,
                                   const char *url, const char *method,
                                   const char *version, const char *upload_data,
                                   size_t *upload_data_size, void **con_cls);
    MHD_Daemon* daemon;
    std::string lastTemp;    
    TemperatureReader temperatureReader;
    GpioReciver gpioReciverFunc1;
    GpioPinReader gpioPinReader1;
    IBasicThreads<GpioReciver> gpioReciver1;
    GPIOWebService(const GPIOWebService&);
    const GPIOWebService& operator=(const GPIOWebService&);
    GPIOWebService& operator=(GPIOWebService&);
    std::string getPage(std::string FileUrl);
    std::condition_variable terminateServiceSignalCondition;
    std::mutex terminateMutex;
    bool terminateFlag;
    const bool terminalPred();
public:
    GPIOWebService();
    ~GPIOWebService();
    void WaitForTerminate();
    
    void StartListen(const int port);
    
};
#endif /* defined(__ThreadPool__GPIOWebService__) */
