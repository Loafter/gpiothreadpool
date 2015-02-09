//
//  GpioSignalCreator.cpp
//  ThreadPool
//
//  Created by Andrew Rose on 02.08.13.
//  Copyright (c) 2013 Andrew Rose. All rights reserved.
//

#include "GpioSignalCreator.h"
#include "GpioSignals.h"
#include <sstream>
#include <iostream>
#include <regex.h>
#include <sys/types.h>
#include <memory>
using namespace std;

//http://andrewrose.flnet.org:443/ActionType="SendSignal"&GpioID="1"&Data="1"
//http://localhost:8888/ActionType="SendSignal"&GpioID="1"&Data="1"


string GpioSignalCreator::findRegex(const string& source,const string&  regexString)
{

    
    size_t maxGroups = 5;
    
    regex_t regexCompiled;
    regmatch_t groupArray[maxGroups];
    
    if (regcomp(&regexCompiled, regexString.c_str(), REG_EXTENDED))
    {
        throw "Could not compile regular expression";
    };
    
    if (regexec(&regexCompiled, source.c_str(), maxGroups, groupArray, 0) == 0)
    {
       int stp=groupArray[1].rm_so;

        if ((stp>=0)&&(stp<source.length()-1))
            {
                
               string retval=source.substr(groupArray[1].rm_so, groupArray[1].rm_eo-groupArray[1].rm_so);
               regfree(&regexCompiled);
               return retval;
            }
        
    }

    regfree(&regexCompiled);
    throw "No find regular expression";
    return 0;
}
SignalTypeEnum GpioSignalCreator::parseActionType(const string& signalString)
{
   const string pattern="ActionType=\"([a-zA-Z]*)\"";
   string actionTypeSt=findRegex(signalString,pattern);
   if(actionTypeSt=="GetTemperature")
       return SignalTypeEnum::GetTemperature;
   else if(actionTypeSt=="GetPinLevel")
       return SignalTypeEnum::GetPinLevel;
   else if(actionTypeSt=="SetHiLevelPin")
       return SignalTypeEnum::SetHiLevelPin;
   else if(actionTypeSt=="SetLowLevelPin")
       return SignalTypeEnum::SetLowLevelPin;
   else if(actionTypeSt=="GetPinLevel")
       return SignalTypeEnum::GetTemperature;
   
    throw "Parce action type error";

}
int GpioSignalCreator::parseGpioPortId(const std::string& signalString)
{
    const string pattern="GpioID=\"([0-9]*)\"";
    
    try
    {
        int retId;
        std::istringstream ist(findRegex(signalString,pattern));
        ist >> retId;
        return retId;
    } catch (...)
    {
       throw "Gpio Port Id parse error";
    }
}



shared_ptr<BasicGpioSignal> GpioSignalCreator::ParseSignal(const std::string& signalString)
{
    switch (parseActionType(signalString))
    {
        case SignalTypeEnum::GetTemperature:
            return shared_ptr<BasicGpioSignal>(new GetTemperatureSignal());
            break;
        case SignalTypeEnum::GetPinLevel:
        {
            int pin=parseGpioPortId(signalString);
            return shared_ptr<BasicGpioSignal>(new ReadLevelGpio(pin));
            break;
        }
        case SignalTypeEnum::SetHiLevelPin:
        {
            int pin=parseGpioPortId(signalString);
            return shared_ptr<BasicGpioSignal>(new SetHiLevelSignal(pin));
            break;
        }
        case SignalTypeEnum::SetLowLevelPin:
        {
            int pin=parseGpioPortId(signalString);
            return shared_ptr<BasicGpioSignal>(new SetLowLevelSignal(pin));
            break;
        }

    }
    throw "Unknown type error";
}
