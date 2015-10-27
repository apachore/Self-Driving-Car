/*
 * Motor_PWM.cpp
 *
 *  Created on: 25-Oct-2015
 *      Author: Su
 */

#include <stdio.h>
#include "Motor.hpp"
#include "lpc_pwm.hpp"
#include "queue.h"

static int Motor_ini;
static int fwd_stp;
void Forward(uint8_t fwd_level);
void Backward(uint8_t rev_level);
void Stop(uint8_t stp);

void Forward(uint8_t fwd_level)
{

}

/*Motor control through PWM signal*/
void Motor_PWM_control()
{
//    /*CAN data bytes received from Master to control motor. Will be replaced soon*/
    can_data_t Mstr_0x220;
    QueueHandle_t Master_Motor_q = scheduler_task::getSharedObject("Motor_Control_q");

    if(NULL == Master_Motor_q)
        {
        printf("Critical Error : Master_Motor_q not received\n");
        }
    else if(xQueueReceive(Master_Motor_q, &Mstr_0x220,1000))
        {
      int byte0 = Mstr_0x220.bytes[0], byte1 = Mstr_0x220.bytes[1], byte2 = Mstr_0x220.bytes[2], byte3 = Mstr_0x220.bytes[3];
//    int byte0=0, byte1 =0, byte2=1, byte3=2;
//    PWM pwm2(PWM::pwm2, 100); /* set pw2 for dc motor */
//    PWM pwm1(PWM::pwm1, 100); /* set pw1 for servo motor */
//    if (Motor_ini == 0)
//    {
//        pwm2.set(15); /* initialization pulse for dc motor */
//        delay_ms(40);
//        Motor_ini = 1;
//    }
//
//    else
//    {
//        switch (byte1)
//        {
//
//            case 0:
//                pwm1.set(15);
//                break;
//
//            case 1:
//                if (byte0 == 1)
//                {
//                    pwm1.set(14);
//                }
//                else if (byte0 == 2)
//                {
//                    pwm1.set(16);
//                    printf("PWM : 16\n");
//                }
//                else
//                {
//                    pwm1.set(15);
//                }
//                break;
//
//            case 2:
//                if (byte0 == 1)
//                {
//                    pwm1.set(13);
//                }
//                else if (byte0 == 2)
//                {
//                    pwm1.set(17);
//                }
//                else
//                {
//                    pwm1.set(15);
//                }
//                break;
//
//            default:
//                pwm1.set(15);
//                break;
//
//        }
//
//        switch (byte3)
//        {
//
//            case 0:
//                pwm2.set(15);
//                break;
//
//            case 1:
//                if (byte2 == 1)
//                {
//                    pwm2.set(16);
//                }
//                else if (byte2 == 2)
//                {
//                    pwm2.set(14);
//                }
//                else
//                {
//                    pwm2.set(15);
//                }
//                break;
//
//            case 2:
//                if (byte2 == 1)
//                {
//                    pwm2.set(16.4);
//                }
//                else if (byte2 == 2)
//                {
//                    pwm2.set(13.6);
//                }
//                else
//                {
//                    pwm2.set(15);
//                }
//                break;
//
//            case 3:
//                if (byte2 == 1)
//                {
//                    pwm2.set(16.8);
//                }
//                else if (byte2 == 2)
//                {
//                    pwm2.set(13.2);
//                }
//                else
//                {
//                    pwm2.set(15);
//                }
//                break;
//
//            default:
//                pwm1.set(15);
//                break;
//        }
//
//    }
    printf("in Motor PWM\n");
        }
}

