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

#include "source/can_transmission_reception.h"


extern bool BootReplySent;
extern uint8_t Received_Checkpoint_Count;
extern uint16_t Total_Distance_To_Travel;
uint16_t current_bearing;

//Commented printf statements are added for testing purpose

gpsTask::gpsTask(uint8_t priority) : scheduler_task("gps", 4*512, priority),
gps_uart(Uart3::getInstance()),
gps_data_q(NULL)
{
    //setRunDuration(10);
    gps_uart.init(gps_uart_BaudRate,gps_rx_q_size,1);
}

bool gpsTask::init(void)
{
    void logger_init();
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

float /*gpsTask::*/ToRadians(float InDegrees)
{
    return(InDegrees*(M_PI/180));
}

float /*gpsTask::*/ToDegrees(float InRadians)
{
    return(InRadians*(180/M_PI));
}

uint16_t /*gpsTask::*/calculateBearing(coordinates point_2,coordinates point_1)
{
    float x,y;
    uint16_t bearing;
    y = sin(ToRadians(point_2.longitude-point_1.longitude))*cos(ToRadians(point_2.latitude));
    x = (cos(ToRadians(point_1.latitude))*sin(ToRadians(point_2.latitude))) - (sin(ToRadians(point_1.latitude))*cos(ToRadians(point_2.latitude))*cos(ToRadians(point_2.longitude-point_1.longitude)));
    bearing = (int)(ToDegrees(atan2(y,x))+360)%360;
    return(bearing);

}
uint16_t /*gpsTask::*/calculateCheckpointDistance(coordinates point_2,coordinates point_1)
{
    float a,c;
    uint16_t distance;
    a = pow(sin(ToRadians(point_2.latitude-point_1.latitude)/2),2) + (cos(ToRadians(point_1.latitude))*cos(ToRadians(point_2.latitude))*pow(sin(ToRadians(point_2.latitude-point_1.latitude)/2),2));
    c = 2 * atan2(sqrt(a),sqrt(1-a));
    distance = Radius * c;
    return(distance);
}

bool gpsTask::run(void *p)
{
    //printf("Inside GPS Task Run\n");
    if( gps_uart.gets(GPS_Data_Receive, sizeof(GPS_Data_Receive),1000))
    {
        //printf("Inside Receive IF Statement\n");
        //printf("Data from GPS: %s\n",GPS_Data_Receive);

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
                //LE.toggle(1);
                //printf("GPS data:%s\n",GPS_Data_Receive);
                //printf("GPS Message type: %s\n",GPS_parsed_data[0]);

                sscanf(GPS_parsed_data[2],"%f",&gps_readings.latitude);
                sscanf(GPS_parsed_data[4],"%f",&gps_readings.longitude);
                //printf("GPS reading Latitude: %f\n",gps_readings.latitude);
                //printf("GPS reading Longitude: %f\n",gps_readings.longitude);

                /*Readings are in 3721.1831,N,12153.5919,W format in which
                 *3721.1831 = 37 degrees and 21.1831 minutes
                 *To convert degree minute format to degrees below calculations are programmed
                 *e.g gps_readings = 3721.1831/100 = 37.211831
                 *floor will give 37 degrees
                 *current_gps_data.latitude = degrees.latitude + (((gps_readings.latitude-degrees.latitude)*100)/60);
                 *current_gps_data.latitude = 37 + (((37.211831 - 37)*100)/60) = 37.353050*/
                gps_readings.latitude /=100;
                gps_readings.longitude /=100;
                degrees.latitude = floor(gps_readings.latitude);
                degrees.longitude = floor(gps_readings.longitude);
                current_gps_data.latitude = degrees.latitude + (((gps_readings.latitude-degrees.latitude)*100)/60);
                current_gps_data.longitude = degrees.longitude + (((gps_readings.longitude-degrees.longitude)*100)/60);

                if (*GPS_parsed_data[3] == 'S')
                    current_gps_data.latitude *= -1;
                if (*GPS_parsed_data[5] == 'W')
                    current_gps_data.longitude *= -1;
//                printf("Latitude: %f\n",current_gps_data.latitude);
//                printf("Longitude: %f\n",current_gps_data.longitude);

                if (!xQueueSend(gps_data_q, &current_gps_data, 0))
                {
                // unexpected led
                   LOG_ERROR("Error in Writing current GPS data to queue\n");
                }

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


void GPS_Calculations()
{
    //gpsTask gpsTaskInstance()
    QueueHandle_t gps_data_q = scheduler_task::getSharedObject("gps_queue");
    QueueHandle_t Checkpoint_q = scheduler_task::getSharedObject("CheckpointQueue");
    coordinates current_gps_data;
    Distance Current_Distances;
    //uint16_t Current_Checkpoint_Distance,Total_Distance_Remaining;

    static uint16_t Previous_Checkpoint_Distnace,Total_Checkpoint_Distnace,Total_Distance_Traveled;
    static coordinates checkpoint;
    static bool Fetch_Checkpoint = 1;
    static bool first_after_cp_fetch;
    static uint8_t Fetched_Checkpoint_Count;

    if (NULL == gps_data_q)
    {
        //light_up_the_project_blown_led();
    }
    else if (xQueueReceive(gps_data_q, &current_gps_data, 0))
    {
        if (BootReplySent)
        {
            //LOG_INFO("Latitude: %f  Longitude: %f ",current_gps_data.latitude,current_gps_data.longitude);
            //LE.toggle(2);
            //printf("Latitude: %f\n",current_gps_data.latitude);
            //printf("Longitude: %f\n",current_gps_data.longitude);

          CANTransmit(TSourceCoordinates,(uint8_t*)&current_gps_data,sizeof(coordinates));

            if (Fetch_Checkpoint)
            {
                if (xQueueReceive(Checkpoint_q, &checkpoint, 0))
                {
                    Fetch_Checkpoint = 0;
                    Fetched_Checkpoint_Count++;
                    first_after_cp_fetch = 1;
                }
            }

            if(!Fetch_Checkpoint)
            {
                Current_Distances.Current_Checkpoint_Distance = calculateCheckpointDistance(checkpoint,current_gps_data);
                //printf("Current Checkpoint Distance: %d feet\n",current_cp_distance);
                current_bearing = calculateBearing(checkpoint,current_gps_data);
                //printf("Bearing: %d degrees\n",current_bearing);
                if (first_after_cp_fetch)
                {
                    Previous_Checkpoint_Distnace = Current_Distances.Current_Checkpoint_Distance;
                    Total_Checkpoint_Distnace = Current_Distances.Current_Checkpoint_Distance;
                    first_after_cp_fetch = 0;
                }

                //printf("Total Checkpoint Distance: %d feet\n",previous_checkpoint_distnace);
                //printf("Fetched_Checkpoint_Count: %d\n",Fetched_Checkpoint_Count);
                //printf("Received Checkpoint Count: %d\n",Received_Checkpoint_Count);
                if (Current_Distances.Current_Checkpoint_Distance <  0.25*Total_Checkpoint_Distnace)
                    Fetch_Checkpoint = 1;

                Total_Distance_Traveled = Total_Distance_Traveled + (Previous_Checkpoint_Distnace - Current_Distances.Current_Checkpoint_Distance);
                Previous_Checkpoint_Distnace = Current_Distances.Current_Checkpoint_Distance;
                Current_Distances.Total_Distance_Remaining = Total_Distance_To_Travel - Total_Distance_Traveled;

                CANTransmit(TFinalAndNextCheckpointDistance,(uint8_t*)&Current_Distances,sizeof(Current_Distances));
            }
        }
    }
}
