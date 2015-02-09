//
//  GPIOSignals.h
//  ThreadPool
//
//  Created by Andrew Rose on 01.08.13.
//  Copyright (c) 2013 Andrew Rose. All rights reserved.
//

#ifndef __ThreadPool__GpioSignals__
#define __ThreadPool__GpioSignals__

#include <iostream>
#include <string>
enum struct SignalTypeEnum
{
	GetTemperature,
    GetPinLevel,
	SetHiLevelPin,
    SetLowLevelPin,
	Service
};

struct BasicGpioSignal
{
public:
    SignalTypeEnum SignalType;
    BasicGpioSignal(SignalTypeEnum signalType):SignalType(signalType)
    {}
};

struct ReadLevelGpio:public BasicGpioSignal
{
public:
    const int PinId;
    ReadLevelGpio(const int pinId):BasicGpioSignal(SignalTypeEnum::GetPinLevel),PinId(pinId){};
    
};

struct GetTemperatureSignal:public BasicGpioSignal
{
public:   
    GetTemperatureSignal():BasicGpioSignal(SignalTypeEnum::GetTemperature){};

};



struct SetHiLevelSignal:public BasicGpioSignal
{
public:
    const int PinId;
    SetHiLevelSignal(const int pinId):BasicGpioSignal(SignalTypeEnum::SetHiLevelPin),PinId(pinId){};
    
};

struct SetLowLevelSignal:public BasicGpioSignal
{
public:
    const int PinId;
    SetLowLevelSignal(const int pinId):BasicGpioSignal(SignalTypeEnum::SetLowLevelPin),PinId(pinId){};
    
};


#endif /* defined(__ThreadPool__GPIOSignals__) */
