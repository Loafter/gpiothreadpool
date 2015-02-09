//
//  GpioSignalCreator.h
//  ThreadPool
//
//  Created by Andrew Rose on 02.08.13.
//  Copyright (c) 2013 Andrew Rose. All rights reserved.
//

#ifndef __ThreadPool__GpioSignalCreator__
#define __ThreadPool__GpioSignalCreator__

#include <iostream>
#include "GpioSignals.h"
#include <memory>

class  GpioSignalCreator
{
private:
 
    static std::string findRegex(const std::string&  source,const std::string& regexString);
    static SignalTypeEnum parseActionType(const std::string& signalString);
    static int parseGpioPortId(const std::string& signalString);
public:
    static std::shared_ptr<BasicGpioSignal> ParseSignal(const std::string& signalString);
};
#endif /* defined(__ThreadPool__GpioSignalCreator__) */
