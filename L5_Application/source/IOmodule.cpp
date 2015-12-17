/*
 * IOmodule.cpp

 *
 *  Created on: 26-Oct-2015
 *      Author: User
 */

#include "IOmodule.hpp"
#include <strings.h>
#include <iostream>

using namespace std;
/*
 *
 * IO task - to print messages on LCD
 *
 * received from sensor and GPS
 *
 *
 */
#include "IOmodule.hpp"


float gps_latitude;
float gps_longitude;
uint16_t final_dist_remaining;
uint16_t checkpoint_dist;
extern Can_msg Message;
extern coordinates checkpoint;
uint8_t sensor_data[4];
uint8_t compass_data[2];
char a13[80];
int clear_index;

Uart2 &u2 = Uart2 ::getInstance();
//Initializing the UART2 with 9600 baudrate
void UARTInitialization()
{
   static const int io_rx_q_size = 32;
   static const int io_tx_q_size = 32;
   u2.init(9600, io_rx_q_size, io_tx_q_size);
   u2.put("FO");
}

//This function displays information every one second
void LCDdisplay()
{
    char dist_buf[4],sen_buf0[4],sen_buf1[4],sen_buf2[4],sen_buf3[4],checkpoint_dist_buf[4],compass_buf0[4],compass_buf1[4];

    char team_name_buf[7]="MINION";
    int i;
    char gps_lat_buf[50],gps_long_buf[50];
    char s=32;

   // printf("int size:%d\t float size:%d\n",sizeof(int),sizeof(float));
    char command_buf[3];
    u2.put("HEOOOOOOO");
   // char checkpoint_dist_buf[4];
    printf("int size:\t float size:\n");
    //Copying Sensor data
   /* sensor_data[0] =Message.canReceivedMsg_Sensor.data.bytes[0];
    sensor_data[1] =Message.canReceivedMsg_Sensor.data.bytes[1];
    sensor_data[2] =Message.canReceivedMsg_Sensor.data.bytes[2];
    sensor_data[3] =Message.canReceivedMsg_Sensor.data.bytes[3];*/
    /*sensor_data[0]=11;
    sensor_data[1]=49;
    sensor_data[2]=255;
    sensor_data[3]=8;*/

    //Reading Compass data
   /* compass_data[0]=Message.canReceivedMsg_compass.data.bytes[0];
    compass_data[1]=Message.canReceivedMsg_compass.data.bytes[1];*/

    //Distance remaining information from read
  //  final_dist_remaining=Message.canReceivedMsg_gps2.data.words[0];
  // checkpoint_dist=Message.canReceivedMsg_gps2.data.words[1];
   /* gps_longitude =24.413;
    gps_latitude=7.89;
    final_dist_remaining=63;
    checkpoint_dist = 34;*/
  // snprintf(checkpoint_dist_buf,4,"%d",checkpoint_dist);

    //Printing Distance remaining, Compass and Sensor data
     //  snprintf(dist_buf,4,"%d",final_dist_remaining);
  //  sprintf(gps_lat_buf,"%0.2f",checkpoint.latitude);
 //   sprintf(gps_long_buf,"%0.2f",checkpoint.longitude);
      /* snprintf(compass_buf0,4,"%d",compass_data[0]);
       snprintf(compass_buf1,4,"%d",compass_data[1]);

    snprintf(sen_buf0,4,"%d",sensor_data[0]);
    snprintf(sen_buf1,4,"%d",sensor_data[1]);
    snprintf(sen_buf2,4,"%d",sensor_data[2]);
    snprintf(sen_buf3,4,"%d",sensor_data[3]);*/
  //  command_buf[0] = 0xFE;  // command to place cursor in line 0  */
  /*  command_buf[1] = 0x45;
    command_buf[2] = 0x04;
    u2.put(command_buf);
    u2.put(team_name_buf);
    command_buf[0] = 0xFE;
    command_buf[1] = 0x45;
    command_buf[2] = 0x40;
    u2.put(command_buf);  // command to place cursor in line 1
    u2.put("F:");
    u2.put(sen_buf0);
    u2.putChar(s);
    u2.put("L:");
    u2.put(sen_buf2);
    u2.putChar(s);
    u2.put("R:");
    u2.put(sen_buf3);
    u2.putChar(s);
    u2.put("B:");
    u2.put(sen_buf1);
    u2.putChar(s);
    command_buf[0] = 0xFE;  // command to place cursor in line 2
    command_buf[1] = 0x45;
    command_buf[2] = 0x14;
    u2.put(command_buf);
    u2.put("FD:");
    u2.put(dist_buf);
    u2.putChar(s);
  //  u2.put("CD:");
   // u2.put(checkpoint_dist_buf);
   // u2.putChar(s);

    command_buf[0] = 0xFE;  // command to place cursor in line 3
    command_buf[1] = 0x45;
    command_buf[2] = 0x54;
    u2.put(command_buf);
    u2.put("TA:");
    u2.put(compass_buf0);
    u2.putChar(s);
    u2.put("DIR:");
    u2.put(compass_buf1);
    u2.putChar(s);
   /* u2.put("LAT:");
    u2.put(gps_lat_buf);
    u2.putChar(s);
    u2.put("LON:");
    u2.put(gps_long_buf);
    u2.putChar(s);*/
    vTaskDelay(800);
   command_buf[0] = 0xFE;     /* clear screen command */
    command_buf[1] = 0x4B;
    command_buf[2] = 0x00;
    u2.put(command_buf);
}







