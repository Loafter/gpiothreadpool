//
//  TemperatureReader.cpp
//  ThreadPool
//
//  Created by Andrew Rose on 06.08.13.
//  Copyright (c) 2013 Andrew Rose. All rights reserved.
//





#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <bcm2835.h>
#include <unistd.h>
#include "TemperatureReader.h"
#include <sstream>
#define MAXTIMINGS 100
#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */
using namespace std;

string floatToString(float f)
{
    stringstream ss (stringstream::in | stringstream::out);
    ss <<f;
    string test = ss.str();
    return string(ss.str());
}

string TemperatureReader::readDHTF(int pin)
{
    int bits[250], data[100];
    int bitidx = 0;
    int counter = 0;
    int laststate = HIGH;
    int j=0;
    
    // Set GPIO pin to output
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
    
    bcm2835_gpio_write(pin, HIGH);
    usleep(500000);  // 500 ms
    bcm2835_gpio_write(pin, LOW);
    usleep(20000);
    
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT);
    
    data[0] = data[1] = data[2] = data[3] = data[4] = 0;
    
    // wait for pin to drop?
for(int i=0;i<3*MAXTIMINGS;++i)
{    
if (bcm2835_gpio_lev(pin) == 1)
    {
        usleep(1);
    }
else break;
}    
    // read data!
    for (int i=0; i< MAXTIMINGS; i++) {
        counter = 0;
        while ( bcm2835_gpio_lev(pin) == laststate) {
            counter++;
            //nanosleep(1);		// overclocking might change this?
            if (counter == 1000)
                break;
        }
        laststate = bcm2835_gpio_lev(pin);
        if (counter == 1000) break;
        bits[bitidx++] = counter;
        
        if ((i>3) && (i%2 == 0)) {
            // shove each bit into the storage bytes
            data[j/8] <<= 1;
            if (counter > 200)
                data[j/8] |= 1;
            j++;
        }
    }
    
    //printf("Data (%d): 0x%x 0x%x 0x%x 0x%x 0x%x\n", j, data[0], data[1], data[2], data[3], data[4]);
    
    if ((j >= 39) &&
        (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) ) {
            float f, h;
            h = data[0] * 256 + data[1];
            h /= 10;
            
            f = (data[2] & 0x7F)* 256 + data[3];
            f /= 10.0;
            if (data[2] & 0x80)  f *= -1;
        
            //printf("Temp =  %.1f *C, Hum = %.1f \%\n", f, h);
        return floatToString(f);;
    }
    
    return string("");
}
string TemperatureReader::readDHTH(int pin)
{
    int bits[250], data[100];
    int bitidx = 0;
    int counter = 0;
    int laststate = HIGH;
    int j=0;
    
    // Set GPIO pin to output
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
    
    bcm2835_gpio_write(pin, HIGH);
    usleep(500000);  // 500 ms
    bcm2835_gpio_write(pin, LOW);
    usleep(20000);
    
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT);
    
    data[0] = data[1] = data[2] = data[3] = data[4] = 0;
    
    // wait for pin to drop?
for(int i=0;i<3*MAXTIMINGS;++i)
{    
if (bcm2835_gpio_lev(pin) == 1)
    {
        usleep(1);
    }
else break;
}
    
    // read data!
    for (int i=0; i< MAXTIMINGS; i++) {
        counter = 0;
        while ( bcm2835_gpio_lev(pin) == laststate) {
            counter++;
            //nanosleep(1);		// overclocking might change this?
            if (counter == 1000)
                break;
        }
        laststate = bcm2835_gpio_lev(pin);
        if (counter == 1000) break;
        bits[bitidx++] = counter;
        
        if ((i>3) && (i%2 == 0)) {
            // shove each bit into the storage bytes
            data[j/8] <<= 1;
            if (counter > 200)
                data[j/8] |= 1;
            j++;
        }
    }
    
    //printf("Data (%d): 0x%x 0x%x 0x%x 0x%x 0x%x\n", j, data[0], data[1], data[2], data[3], data[4]);
    
    if ((j >= 39) &&
        (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) ) {
        float f, h;
        h = data[0] * 256 + data[1];
        h /= 10;
        
        f = (data[2] & 0x7F)* 256 + data[3];
        f /= 10.0;
        if (data[2] & 0x80)  f *= -1;
        
        //printf("Temp =  %.1f *C, Hum = %.1f \%\n", f, h);
        return floatToString(h);
    }
    
    return string("");
}

TemperatureReader::TemperatureReader(const int pinNumber_):pinNumber(pinNumber_)
{
    bcm2835_init();
}

string TemperatureReader::GetTemperature()
{
    return readDHTF(pinNumber);
}
string TemperatureReader::GetHumidity()
{
    return readDHTH(pinNumber);
    
}
