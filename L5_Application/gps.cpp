/*===========================================================================================
 Name        : gps.cpp
 Author      : Amit Pachore
 Version     : 1.0
 Description : Program to Get current coordinates form GPS Module,
               Calculate Distance,
=============================================================================================
*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "gps.hpp"
#include "eint.h"
#include "uart3.hpp"
#include "file_logger.h"
#include "io.hpp"

//Commented printf statements are added for testing purpose


#define gps_uart_BaudRate 38400

gpsTask::gpsTask(uint8_t priority) : scheduler_task("gps", 4*512, priority),
gps_uart(Uart3::getInstance()),
gps_data_q(NULL)
{
    //setRunDuration(10);
    gps_uart.init(gps_uart_BaudRate,gps_rx_q_size,1);
}

bool gpsTask::init(void)
{
    gps_data_q = xQueueCreate(2, sizeof(coordinates));
    addSharedObject("gps_queue", gps_data_q);

    return (NULL != gps_data_q);
}


//Function moved inside run
//void gpsTask::parse_gps_string(void)
//{
//
//
////        for(i=0;i<14;i++)
////        {
////            printf("%s\n",GPS_parsed_data[i]);
////        }
//}

float calculateBearing()
{

}
float calculateDistance()
{

}

bool gpsTask::run(void *p)
{
    //printf("Inside GPS Task Run\n");
    if( gps_uart.gets(GPS_Data_Receive, sizeof(GPS_Data_Receive),1000))
    {
        //printf("Inside Receive IF Statement\n");
        printf("Data from GPS: %s\n",GPS_Data_Receive);

        uint8_t i=0;
        coordinates degrees;
        char *token;

        //Parse Received String and get string data in variables
        token = strtok(GPS_Data_Receive,",");
        while (token != NULL)
        {
            GPS_parsed_data[i] = token;
            token = strtok(NULL,",");
            i++;
        }

//        for(i=0;i<14;i++)
//        {
//            printf("%d : %s\n",i,GPS_parsed_data[i]);
//        }

//        Previously function for parsing implemented but later parsing was implemented in run
//        parse_gps_string();
//        //sscanf(GPS_Data_Receive, "%s,%*s,%f,%c,%f,%c",&message_type, &current_gps_data.latitude,&North_South_Hemisphere, &current_gps_data.longitude,&East_West_Hemisphere);

        //Get number of satellite locks
        sscanf(GPS_parsed_data[7],"%d",&satelite_connections);
        LD.setNumber(satelite_connections);

        /* If number satellite locks are valid then only proceed with coordinate
         * conversion to degrees and sending it over queue*/
        if (satelite_connections>2)
        {
            // Condition inserted before setting up module to Only GGA messages
            //if (!strcmp(GPS_parsed_data[0],"$GPGGA"))
            //{
                LE.toggle(1);
                //printf("GPS data:%s\n",GPS_Data_Receive);
                //printf("GPS Message type: %s\n",GPS_parsed_data[0]);

                sscanf(GPS_parsed_data[2],"%f",&gps_readings.latitude);
                sscanf(GPS_parsed_data[4],"%f",&gps_readings.longitude);
                //printf("GPS reading Latitude: %f\n",gps_readings.latitude);
                //printf("GPS reading Longitude: %f\n",gps_readings.longitude);

                gps_readings.latitude /=100;
                gps_readings.longitude /=100;
                degrees.latitude = floor(gps_readings.latitude);
                degrees.longitude = floor(gps_readings.longitude);
                current_gps_data.latitude = degrees.latitude + (((gps_readings.latitude-degrees.latitude)*100)/60);
                current_gps_data.longitude = degrees.longitude + (((gps_readings.longitude-degrees.longitude)*100)/60);

                //current_gps_data.latitude = atof(GPS_parsed_data[2]);
                //current_gps_data.longitude = atof(GPS_parsed_data[4]);
                if (*GPS_parsed_data[3] == 'S')
                    current_gps_data.latitude *= -1;
                if (*GPS_parsed_data[5] == 'W')
                    current_gps_data.longitude *= -1;
                printf("Latitude: %f\n",current_gps_data.latitude);
                printf("Longitude: %f\n",current_gps_data.longitude);
                //if (North_South_Hemisphere == 'S')
                //    current_gps_data.latitude *=-1;
                //if (East_West_Hemisphere == 'W')
                //    current_gps_data.longitude *=-1;


                //if (!xQueueSend(gps_data_q, &current_gps_data, 0))
                //{
                //// unexpected led
                //   LOG_ERROR("Error in Writing current GPS data to queue\n");
                //}

//            }
//            else
//            {
//                printf("Message Other than GPGGA\n");
//            }
        }

    }
    else {
//        // error case
    }
    return true;
}
