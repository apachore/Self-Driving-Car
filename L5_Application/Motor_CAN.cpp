/*
 * Motor.cpp
 *
 *  Created on: 25-Oct-2015
 *      Author: Su
 */
#include <stdio.h>
#include "Motor.hpp"
#include "can.h"


void Bus_Off()
{
    printf("  Error:  Bus_Off\n");
    CAN_reset_bus(can1);
}

void Data_Over_Run()
{
    printf("  Error:  Data_Over_Run\n");
    CAN_reset_bus(can1);
}


