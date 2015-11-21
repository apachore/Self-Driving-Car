/*===========================================================================================
 Name        : gps.hpp
 Author      : Amit Pachore
 Version     : 1.0
 Description : Header file for class declaration of GPS task
=============================================================================================
*/

#ifndef L5_APPLICATION_GPS_HPP_
#define L5_APPLICATION_GPS_HPP_

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "scheduler_task.hpp"
#include "Uart3.hpp"
#include "eint.h"
#include "file_logger.h"
#include "io.hpp"
#include "storage.hpp"
#include "handlers.hpp"

#include "source/can_transmission_reception.h"

#define Radius 20902231.64 //feet
#define gps_uart_BaudRate 38400
#define BootReplyData 20151107

typedef struct
{
        float latitude;
        float longitude;
}coordinates;

typedef struct
{
        uint16_t Total_Distance_Remaining;
        uint16_t Current_Checkpoint_Distance;
}Distance;


uint16_t calculateBearing(coordinates,coordinates);
uint16_t calculateCheckpointDistance(coordinates,coordinates);
float ToRadians(float);
float ToDegrees(float);

void GPS_Calculations();
void copyLogtoMMCard();
bool crtNewLogFile();

class gpsTask : public scheduler_task
{

    public:
        //char message_type[10];
        gpsTask(uint8_t priority);
        bool init(void);
        //void parse_gps_string(void);
        bool run(void *p);

    private:
        char GPS_Data_Receive[100];
//        char North_South_Hemisphere = 0;
//        char East_West_Hemisphere = 0;
        char *GPS_parsed_data[14];
        coordinates current_gps_data,gps_readings;
        uint8_t satelite_connections=0;
        Uart3 &gps_uart;
        static const int gps_rx_q_size = 100;

        QueueHandle_t gps_data_q;
};


#endif /* L5_APPLICATION_GPS_HPP_ */
