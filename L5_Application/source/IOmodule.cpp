/*
 * IOmodule.cpp

 *
 *  Created on: 26-Oct-2015
 *      Author: User
 */

#include "IOmodule.hpp"

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
int final_dist_remaining;
extern Can_msg Message;
uint8_t sensor_data[4];
char a13[80];
int clear_index;
int init =240;
char d;


Uart2 &u2 = Uart2 ::getInstance();

void UARTInitialization()
{
   static const int io_rx_q_size = 32;
   static const int io_tx_q_size = 32;
   u2.init(9600, io_rx_q_size, io_tx_q_size);
 //  d = 240 +'0';
 //  u2.putchar(d);
 //  u2.put("FO");
   for(clear_index=0;clear_index<80;clear_index++)
   {
       a13[clear_index]=32;
   }
 //  u2.put(a13);

  // u2.put("MINION");
}
void LCDdisplay()
{
    char a1[6],a2[6],a3[6],a4[3],a5[3],a6[3],a7[3],a8[10],a11[5];
    char w1[20]="MINION";
    int i;
    char s=32;
 /* sensor_data[0] =Message.canReceivedMsg_Sensor.data.bytes[0];
    sensor_data[1] =Message.canReceivedMsg_Sensor.data.bytes[1];
    sensor_data[2] =Message.canReceivedMsg_Sensor.data.bytes[2];
    sensor_data[3] =Message.canReceivedMsg_Sensor.data.bytes[3];*/
   // printf("front\%f\n",sensor_data[0]);
    sensor_data[0]=88;
    sensor_data[1]=126;
    sensor_data[2]=9;
    sensor_data[3]=8;
 // gps_latitude=Message.canReceivedMsg_gps1.data.dwords[0];
 // printf("gps_lat\%f\n",gps_latitude);
 //   gps_longitude=Message.canReceivedMsg_gps1.data.dwords[1];
   // final_dist_remaining=Message.canReceivedMsg_gps2.data.words[0];
    gps_longitude =24.45;
    gps_latitude=7.89;
    final_dist_remaining=63;
    snprintf(a1,6,"%f",gps_latitude);
    snprintf(a2,6,"%f",gps_longitude);
    snprintf(a3,3,"%d",final_dist_remaining);
    snprintf(a4,4,"%d",sensor_data[0]);
    snprintf(a5,4,"%d",sensor_data[1]);
    snprintf(a6,4,"%d",sensor_data[2]);
    snprintf(a7,4,"%d",sensor_data[3]);
    char temp[3];
    temp[0] = 0xFE;
    temp[1] = 0x45;
    temp[2] = 0x07;
    u2.put(temp);
    u2.put(w1);
    temp[0] = 0xFE;
    temp[1] = 0x45;
     temp[2] = 0x40;
     u2.put(temp);
     u2.put("F:");
         u2.put(a4);
         u2.putChar(s);
         u2.put("B:");
         u2.put(a5);
         u2.putChar(s);
         u2.put("L:");
         u2.put(a6);
         u2.putChar(s);
         u2.put("R:");
         u2.put(a7);
         u2.putChar(s);
         temp[0] = 0xFE;
          temp[1] = 0x45;
          temp[2] = 0x14;
          u2.put(temp);
    u2.put("LT:");
    u2.put(a1);
    u2.putChar(s);
    u2.put("LG:");
    u2.put(a2);
    u2.putChar(s);
    temp[0] = 0xFE;
     temp[1] = 0x45;
      temp[2] = 0x54;
      u2.put(temp);
    u2.put("DS:");
    u2.put(a3);
    u2.putChar(s);
    vTaskDelay(800);
    u2.put(a13);
}






