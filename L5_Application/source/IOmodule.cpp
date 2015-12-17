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


uint16_t final_dist_remaining;
uint16_t checkpoint_dist;
extern Can_msg Message;
extern coordinates checkpoint;
uint8_t sensor_data[4];
uint8_t compass_data[2];
char a13[80];
int clear_index;
char clear_command_buf[3];
char second_line_buf[3];
char baud_rate_buff[3];

extern bool dest_reached_flag;
Uart2 &u2 = Uart2 ::getInstance();
//Initializing the UART2 with 9600 baudrate
void UARTInitialization()
{
   char a[1];
   static const int io_rx_q_size = 32;
   static const int io_tx_q_size = 32;
   u2.init(19200, io_rx_q_size, io_tx_q_size);

   a[0] = 0xF0;
   u2.put(a);

  /* clear_command_buf[0] = 0xFE;     /* clear screen command */
  /* clear_command_buf[1] = 0x51;
   clear_command_buf[2] = 0x00;
   second_line_buf[0] = 0xFE;  // command to place cursor in line 2
   second_line_buf[1] = 0x45;
   second_line_buf[2] = 0x14;*/
  /* baud_rate_buff[0] = 0x7C;     /* clear screen command */
  /* baud_rate_buff[1] = 0x07;
   baud_rate_buff[2] = 0x33;
   u2.put(baud_rate_buff);*/

}

//This function displays information every one second
void LCDdisplay()
{
    char dist_buf[4],sen_buf0[4],sen_buf1[4],sen_buf2[4],sen_buf3[4],checkpoint_dist_buf[6],compass_buf0[4],compass_buf1[4];
    char gps_lat_buf[50],gps_long_buf[50];
    char s=32;
  /*  sensor_data[0]=111;
    sensor_data[1]=49;
    sensor_data[2]=255;
    sensor_data[3]=8;
    final_dist_remaining=63;
    checkpoint_dist = 34;*/
      sensor_data[0] =Message.canReceivedMsg_Sensor.data.bytes[0];
        sensor_data[1] =Message.canReceivedMsg_Sensor.data.bytes[1];
        sensor_data[2] =Message.canReceivedMsg_Sensor.data.bytes[2];
        sensor_data[3] =Message.canReceivedMsg_Sensor.data.bytes[3];
    final_dist_remaining=Message.canReceivedMsg_gps2.data.words[0];
       checkpoint_dist=Message.canReceivedMsg_gps2.data.words[1];
    sprintf(sen_buf0,"%d",sensor_data[0]);
    sprintf(sen_buf1,"%d",sensor_data[1]);
    sprintf(sen_buf2,"%d",sensor_data[2]);
    sprintf(sen_buf3,"%d",sensor_data[3]);
   sprintf(checkpoint_dist_buf,"%d",checkpoint_dist);
    sprintf(dist_buf,"%d",final_dist_remaining);
    u2.putline("$GOTO:7:0");
    u2.putline("MINION");
    u2.putline("$GOTO:0:1");
    u2.putline("F:");
    u2.putline("$GOTO:2:1");
    u2.putline(sen_buf0);
    u2.putChar(s);
    u2.putline("$GOTO:6:1");
    u2.putline("L:");
    u2.putline("$GOTO:8:1");
    u2.putline(sen_buf2);
    u2.putChar(s);
    u2.putline("$GOTO:12:1");
    u2.putline("R:");
    u2.putline("$GOTO:14:1");
    u2.putline(sen_buf3);
    u2.putChar(s);
    u2.putline("$GOTO:0:2");
    u2.putline("B:");
    u2.putline("$GOTO:2:2");
    u2.putline(sen_buf1);
    u2.putChar(s);
    u2.putline("$GOTO:6:2");
    u2.putline("FD:");
    u2.putline("$GOTO:9:2");
    u2.putline(dist_buf);
    u2.putChar(s);
    u2.putline("$GOTO:12:2");
    u2.putline("CD:");
    u2.putline("$GOTO:15:2");
    u2.put(checkpoint_dist_buf);
    u2.putChar(s);
  //  compass_data[0]=Message.canReceivedMsg_compass.data.bytes[0];
    //compass_data[1]=Message.canReceivedMsg_compass.data.bytes[1];

    //Distance remaining information from read

   /* gps_longitude =24.413;
    gps_latitude=7.89;*/



    //Printing Distance remaining, Compass and Sensor data

  //  sprintf(gps_lat_buf,"%0.2f",checkpoint.latitude);
 //   sprintf(gps_long_buf,"%0.2f",checkpoint.longitude);
   /*    snprintf(compass_buf0,4,"%d",compass_data[0]);
       snprintf(compass_buf1,4,"%d",compass_data[1]);*/



 /*

    u2.putChar(s);
    u2.put("CD:");

   // u2.putChar(s);


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
    u2.putline("$CLR_SCR");

}







