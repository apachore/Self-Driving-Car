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
#include "stdlib.h"
#include "stdio.h"
#include "IOmodule.hpp"

extern float gps_latitude;
extern float gps_longitude;
extern int final_dist_remaining;
extern uint8_t sensor_data[4];

Uart2 &u2 = Uart2 ::getInstance();

void UARTInitialization()
{
puts("uart init\n");
   static const int io_rx_q_size = 32;
   static const int io_tx_q_size = 32;
   u2.init(9600, io_rx_q_size, io_tx_q_size);
   u2.putline("FO");

}
void LCDdisplay()
{
    char a1[6],a2[6],a3[6],a4[3],a5[3],a6[3],a7[3],a8[10];
    sensor_data[0]=88;
    sensor_data[1]=126;
    sensor_data[2]=9;
    sensor_data[3]=8;
    gps_longitude =24.45;
    gps_latitude=7.89;
    final_dist_remaining=63;
    snprintf(a1,5,"%f",gps_latitude);
    snprintf(a2,6,"%f",gps_longitude);
    snprintf(a3,3,"%d",final_dist_remaining);
    snprintf(a4,4,"%d",sensor_data[0]);
    snprintf(a5,4,"%d",sensor_data[1]);
    snprintf(a6,4,"%d",sensor_data[2]);
    snprintf(a7,4,"%d",sensor_data[3]);
    u2.putline("F_dist");
    u2.putline(a4);
    u2.putline("Rr_dist");
    u2.putline(a5);
    u2.putline("L_dist");
    u2.putline(a6);
    u2.putline("Rt_dist");
    u2.putline(a7);
    u2.putline("gps_lat");
    u2.putline(a1);
    u2.putline("gps_long");
    u2.putline(a2);
    u2.putline("dist_remain");
    u2.putline(a3);

}






