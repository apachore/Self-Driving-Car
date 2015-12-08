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
uint8_t sensor_data[4];
char a13[80];
int clear_index;

Uart2 &u2 = Uart2 ::getInstance();

void UARTInitialization()
{
   static const int io_rx_q_size = 32;
   static const int io_tx_q_size = 32;
   u2.init(9600, io_rx_q_size, io_tx_q_size);
   u2.put("FO");
}
void LCDdisplay()
{
    char gps_lat_buf[6],gps_long_buf[6],dist_buf[3],sen_buf0[4],sen_buf1[4],sen_buf2[4],sen_buf3[4];
    char team_name_buf[7]="MINION";
    int i;
    char s=32;
    char command_buf[3];
    char checkpoint_dist_buf[3];
  /* sensor_data[0] =Message.canReceivedMsg_Sensor.data.bytes[0];
    sensor_data[1] =Message.canReceivedMsg_Sensor.data.bytes[1];
    sensor_data[2] =Message.canReceivedMsg_Sensor.data.bytes[2];
    sensor_data[3] =Message.canReceivedMsg_Sensor.data.bytes[3];*/
    sensor_data[0]=11;
    sensor_data[1]=41;
    sensor_data[2]=255;
    sensor_data[3]=8;
  /*gps_latitude=Message.canReceivedMsg_gps1.data.dwords[0];
    gps_longitude=Message.canReceivedMsg_gps1.data.dwords[1];
    final_dist_remaining=Message.canReceivedMsg_gps2.data.words[0];
    checkpoint_dist=Message.canReceivedMsg_gps2.data.words[1];*/
    gps_longitude =24.453;
    gps_latitude=7.89;
    final_dist_remaining=63;
    checkpoint_dist = 34;
    snprintf(gps_lat_buf,6,"%0.2f",gps_latitude);
    snprintf(gps_long_buf,6,"%0.2f",gps_longitude);
    snprintf(checkpoint_dist_buf,3,"%d",checkpoint_dist);
    snprintf(dist_buf,3,"%d",final_dist_remaining);
    snprintf(sen_buf0,4,"%d",sensor_data[0]);
    snprintf(sen_buf1,4,"%d",sensor_data[1]);
    snprintf(sen_buf2,4,"%d",sensor_data[2]);
    snprintf(sen_buf3,4,"%d",sensor_data[3]);
    command_buf[0] = 0xFE;  // command to place cursor in line 0  */
    command_buf[1] = 0x45;
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
    u2.put("CD:");
    u2.put(checkpoint_dist_buf);
    u2.putChar(s);
    u2.put("FD:");
    u2.put(dist_buf);
    u2.putChar(s);
    command_buf[0] = 0xFE;  // command to place cursor in line 3
    command_buf[1] = 0x45;
    command_buf[2] = 0x54;
    u2.put(command_buf);
    u2.put("LAT:");
    u2.put(gps_lat_buf);
    u2.putChar(s);
    u2.put("LON:");
    u2.put(gps_long_buf);
    u2.putChar(s);
    vTaskDelay(800);
    command_buf[0] = 0xFE;     /* clear screen command */
    command_buf[1] = 0x51;
    command_buf[2] = 0x00;
    u2.put(command_buf);
}







