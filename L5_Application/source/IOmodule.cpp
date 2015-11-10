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
#include <stdlib.h>
#include "stdio.h"
#include "IOmodule.hpp"
extern uint32_t gps_data[4];
char char_val[33];
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
    int b1,b2,b3,b4,i,c1,c2,c3,c4;
    sensor_data[0]=34;
    sensor_data[1]=121;
    sensor_data[2]=9;
    sensor_data[3]=8;

    char Front_Dist[] = "F Dist :    ";
    char Rear_Dist[] = "R Dist :    ";
    char Left_Dist[] = "L Dist :    ";
    char Right_Dist[] = "R Dist :    ";
    for(i=0;i<3;i++)
    {
        b1=sensor_data[0]%10;
        b2=sensor_data[1]%10;
        b3=sensor_data[2]%10;
        b4=sensor_data[3]%10;
        Front_Dist[9-i] = b1 + '0';
        Rear_Dist[9-i] = b2 + '0';
        Left_Dist[9-i] = b3 + '0';
        Right_Dist[9-i] = b4 + '0';
        sensor_data[0]=sensor_data[0]/10;
        sensor_data[1]=sensor_data[1]/10;
        sensor_data[2]=sensor_data[2]/10;
        sensor_data[3]=sensor_data[3]/10;
    }

    puts(Front_Dist);
    u2.putline(Front_Dist);
    u2.putline(Rear_Dist);
    u2.putline(Left_Dist);
    u2.putline(Right_Dist);
}






