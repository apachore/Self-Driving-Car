/*
 * Motor.cpp
 *
 *  Created on: 02-Nov-2015
 *      Author: Sudheer
 */

#include <motor.hpp>
#include <stdio.h>
#include "utilities.h"
#include "can.h"
#include "io.hpp"



MotorTask::MotorTask(uint8_t priority) :
        scheduler_task("MotorTask", 512 * 4, priority), pwm2(PWM::pwm2, 100), pwm1(PWM::pwm1, 100)
{
    count_init = 1;
    count_rev = 1;
    just = 0;
    setRunDuration(100);
}

bool MotorTask::init(void)
{
    pwm2.set(15);   /*Initialization sequence for DC motor*/
    delay_ms(100);
    pwm1.set(15);
    delay_ms(100);
    return true;
}

bool MotorTask::run(void *p)
{
    int servo_control, servo_level, DC_control, DC_level;
    float speed_factor, speed_offset, PWM_value_DC, PWM_value_servo;
    static float PWM_value_DC_last, PWM_value_servo_last;
    static QueueHandle_t receivedDataQueue = scheduler_task::getSharedObject("MasterMotorQueue");
    can_msg_t canReceivedData;

    if (count_init)
    {
        config_init();
        count_init--;
    }

    if (xQueueReceive(receivedDataQueue, &canReceivedData, 0))
    {

        LE.toggle(4);
        servo_control = canReceivedData.data.bytes[0];
        servo_level = canReceivedData.data.bytes[1];
        DC_control = canReceivedData.data.bytes[2];
        DC_level = canReceivedData.data.bytes[3];

        printf("\nDC: Control %x, Level %x",servo_control,servo_level);
        printf("\nServo: Control %x, Level %x",DC_control,DC_level);

#if(0)
        if (just < 5)
        {
            LE.toggle(4);
            servo_control = 1; //canReceivedData.data.bytes[0];
            servo_level = 1;//canReceivedData.data.bytes[1];
            DC_control = 1;//canReceivedData.data.bytes[2];
            DC_level = 2;//canReceivedData.data.bytes[3];
            count_rev++;
            just++;
        }
        else
        {
            LE.toggle(3);
            servo_control = 1; //canReceivedData.data.bytes[0];
            servo_level = 1;//canReceivedData.data.bytes[1];
            DC_control = 2;//canReceivedData.data.bytes[2];
            DC_level = 2;//canReceivedData.data.bytes[3];
            just++;

            if(just >= 10)
            {
                just = 0;
            }
        }
#endif

        if((DC_control == 2) && count_rev)
        {
            pwm2.set(15);
            delay_ms(100);
            pwm2.set(14.5);
            delay_ms(100);
            pwm2.set(15);
            delay_ms(100);
            pwm2.set(14.5);
            delay_ms(100);
            count_rev = 0;
        }

        switch(DC_control)
        {
            case 0:
            speed_offset = 0;
            speed_factor =0;
            count_rev = 1;
            break;
            case 1:
            speed_offset = 0.8;
            speed_factor = 0.2;
            count_rev = 1;
            break;
            case 2:
            speed_offset = -0.8;
            speed_factor = -0.2;
            break;
            default:
            speed_offset = 0;
            speed_factor =0;
            count_rev = 1;
            break;
        }
        PWM_value_DC = 15 + (speed_factor)*(DC_level) + speed_offset;

        switch(servo_control)
        {
            case 0:
            speed_offset = 0;
            speed_factor =0;
            break;
            case 1:
            speed_offset = 0.6;
            speed_factor = 0.2;
            break;
            case 2:
            speed_offset = -0.6;
            speed_factor = -0.2;
            break;
            default:
            speed_offset = 0;
            speed_factor =0;
            break;
        }
        PWM_value_servo = 15.0 + (speed_factor)*(servo_level) + speed_offset;

        pwm1.set(PWM_value_servo);
        pwm2.set(PWM_value_DC);

//        delay_ms(2000);

//        printf("\nDC value: %f\nServo value: %f",PWM_value_DC,PWM_value_servo);
    }
    else
    {
 //       LE.toggle(1);
    }
    return true;
}

void MotorTask::config_init(void)
{
    delay_ms(3000);

    if (SW.getSwitch(1) == 1)
    {
        LE.set(1, 1);
        delay_ms(500);
        LE.set(1, 0);

        pwm2.set(15);
        delay_ms(500);
        pwm2.set(16);
        delay_ms(500);
        pwm2.set(17);
        delay_ms(500);
        pwm2.set(18);
        delay_ms(500);
        pwm2.set(19);
        delay_ms(500);
        pwm2.set(20);
        delay_ms(500);
        delay_ms(2000);
    }

    if (SW.getSwitch(4) == 1)
    {
        LE.set(4, 1);
        delay_ms(500);
        LE.set(4, 0);

        pwm2.set(15);
        delay_ms(500);
        pwm2.set(14);
        delay_ms(500);
        pwm2.set(13);
        delay_ms(500);
        pwm2.set(12);
        delay_ms(500);
        pwm2.set(11);
        delay_ms(500);
        pwm2.set(10);
        delay_ms(500);

        delay_ms(1000);
    }
    pwm2.set(15);
}

void MotorTask::set_rev(void)
{

}
