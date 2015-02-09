//
//  GPIOWebService.cpp
//  ThreadPool
//
//  Created by Andrew Rose on 05.08.13.
//  Copyright (c) 2013 Andrew Rose. All rights reserved.
//

#include "GPIOWebService.h"
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>
#include "GpioSignalCreator.h"
#include "DummyReciver.h"
#include <condition_variable>
#include <memory>
#include <exception>
#include <fstream>
using namespace std;
GPIOWebService::GPIOWebService():temperatureReader(4),gpioReciverFunc1(17),gpioReciver1(gpioReciverFunc1),gpioPinReader1(24)
{
                          
}

string GPIOWebService::getPage(string FileUrl)
{
    FileUrl="/home/pi/gpio/index.html";
    ifstream fin(FileUrl);
    string str((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
    return str;
}


string intToString(int i)
{
    stringstream ss (stringstream::in | stringstream::out);
    ss <<i;
    string test = ss.str();
    return string(ss.str());
}



int GPIOWebService::reciverSignalMethod(void *cls, struct MHD_Connection *connection,
                                        const char *url, const char *method,
                                        const char *version, const char *upload_data,
                                        size_t *upload_data_size, void **con_cls)
{
    GPIOWebService* this_=reinterpret_cast<GPIOWebService*>(cls);
    string retval;
    string sturl(url);
    if(!(std::string::npos == sturl.find("Index.html")))
    {
        try
        {
            retval=this_->getPage("/index.html");
        } catch (...)
        {
           retval=string("Page not found")+sturl;
        }
        
        struct MHD_Response* response=MHD_create_response_from_buffer (retval.length(), (void*) retval.c_str(),MHD_RESPMEM_PERSISTENT);
        MHD_add_response_header (response, "Content-Type","text/html");
        MHD_add_response_header(response, "Content-length", intToString(retval.length()).c_str());
        int ret=MHD_queue_response (connection, MHD_HTTP_OK, response);
        MHD_destroy_response (response);
        return ret;

    }
    else if(!(std::string::npos == sturl.find("/Exit/")))
    {
        this_->terminateFlag=true;
        this_->terminateServiceSignalCondition.notify_all();
    }
    else if(!(std::string::npos == sturl.find("/Action/")))
    {
        
        try
         {
             shared_ptr<BasicGpioSignal> recivedSignal=GpioSignalCreator::ParseSignal(url);
             GPIOWebService* this_=reinterpret_cast<GPIOWebService*>(cls);
             if(recivedSignal.get()->SignalType==SignalTypeEnum::GetTemperature)
             {
                 retval=this_->temperatureReader.GetTemperature();
                 if(retval==string(""))
                     retval=this_->lastTemp;
                 else
                     this_->lastTemp=retval;
                 //retval=string("Temperature=")+retval;
	     
             }
             else if(recivedSignal.get()->SignalType==SignalTypeEnum::GetPinLevel)
             {
                
                 int pinState=this_->gpioPinReader1.GetPinState();
                 retval="PinState="+intToString(pinState);
             }
             else if((recivedSignal.get()->SignalType==SignalTypeEnum::SetHiLevelPin)||(recivedSignal.get()->SignalType==SignalTypeEnum::SetLowLevelPin))
             {
                 this_->gpioReciver1.ReciveSignal(recivedSignal);
             }
                 
         }
         catch (exception& e)
         {
             retval=string("Standard exception: ")+e.what();
         }
         catch (const char* extext)
         {
             retval=string("Parse exception: ")+ extext;
         }

     }
	else
  	 retval=string("Unknown command");
        struct MHD_Response* response=MHD_create_response_from_buffer (retval.size(), (void*) retval.c_str(),MHD_RESPMEM_PERSISTENT);
        MHD_add_response_header (response, "Content-Type","text/plain");
        MHD_add_response_header(response, "Content-length", intToString(retval.size()).c_str());
        int ret=MHD_queue_response (connection, MHD_HTTP_OK, response);
        MHD_destroy_response (response);
        return ret;

    
}

void GPIOWebService::StartListen(const int port)
{
    daemon = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION, port, NULL, NULL,&GPIOWebService::reciverSignalMethod, this, MHD_OPTION_END);
        if (NULL == daemon)
            throw "Strat server failed";
}
GPIOWebService::~GPIOWebService()
{
    MHD_stop_daemon(daemon);
}

const bool GPIOWebService::terminalPred()
{
    return terminateFlag;
}
void GPIOWebService::WaitForTerminate()
{
    {
        std::unique_lock<std::mutex> lk(terminateMutex);
        //terminateServiceSignalCondition.wait(lk, []{return terminateFlag;});
        terminateServiceSignalCondition.wait(lk,bind(mem_fun(&GPIOWebService::terminalPred), this));
    }
}
