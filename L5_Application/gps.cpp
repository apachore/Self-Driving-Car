/*========================================
 Name        : gps.cpp
 Author      : Amit Pachore
 Version     : 1.0
 Description : Program to Calculate Distance,
==========================================
*/

#include <stdio.h>
#include <math.h>
#include <string.h>

#include "gps.hpp"
#include "eint.h"
#include "uart3.hpp"

#define gpsBaudRate 9600

gpsTask::gpsTask(uint8_t priority):scheduler_task("gps", 4*512, priority)
{
    Uart3& u3 = Uart3::getInstance();
    u3.init(gpsBaudRate);
}

void gpsTask::ParseString()
{
    int i = 0;
    char *token;
    token = strtok(GPS_Data_Receive,",");
    while (token != NULL)
    {
        Parsed_GPS_String[i] = token;
        token = strtok(NULL,",");
        i++;
    }
    for(i=0;i<13;i++)
        printf("Parsed_GPS_String : %s\n",Parsed_GPS_String[i]);
}

bool gpsTask::run(void *p)
{
    Uart3& u03 = Uart3::getInstance();
    u03.gets(GPS_Data_Receive,100);
    printf("GPS Data: %s\n",GPS_Data_Receive);
    ParseString();
    return true;
}
