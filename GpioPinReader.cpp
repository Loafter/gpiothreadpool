//
//  GpioPinReader.cpp
//  ThreadPool
//
//  Created by Andrew Rose on 13.08.13.
//  Copyright (c) 2013 Andrew Rose. All rights reserved.
//

#include "GpioPinReader.h"
#include <bcm2835.h>
#include <stdio.h>
int GpioPinReader::GetPinState()
{
 bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT);
 return bcm2835_gpio_lev(pin);
}
