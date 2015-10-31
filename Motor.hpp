/*
 * Motor.hpp
 *
 *  Created on: 25-Oct-2015
 *      Author: Su
 */

#ifndef MOTOR_HPP_
#define MOTOR_HPP_
#include <stdio.h>
#include "scheduler_task.hpp"
#include "lpc_pwm.hpp"
#include "utilities.h"
#include "can.h"
#include "io.hpp"
#include "can_comms.hpp"


void Motor_PWM_control();

void Bus_Off();

void Data_Over_Run();





class MotorTask : public scheduler_task
{
    public:
    MotorTask(uint8_t priority):scheduler_task("MotorTask", 512*4, priority), pwm2(PWM::pwm2, 100), pwm1(PWM::pwm1, 100)
{
        setRunDuration(1);
/*Nothing to init*/

}
    bool init(void)
    {

      return true;
    }
    bool run(void *p)
    {
        int servo_control,servo_level,DC_control,DC_level;
        float speed_factor, speed_offset, PWM_value_DC,PWM_value_servo,neutral=15.0;
        static int count = 0;
        static QueueHandle_t receivedDataQueue = scheduler_task::getSharedObject("MasterMotorQueue");
        can_msg_t canReceivedData;

        if (xQueueReceive(receivedDataQueue, &canReceivedData, 0))
        {
            LE.toggle(4);
            servo_control = canReceivedData.data.bytes[0];
            servo_level = canReceivedData.data.bytes[1];
            DC_control = canReceivedData.data.bytes[2];
            DC_level = canReceivedData.data.bytes[3];

            if(count==0)
            {
                pwm2.set(15);
                count++;
            }

            else
            {
                switch(canReceivedData.data.bytes[2])
                {
                    case 0:
                        speed_offset = 0;
                        speed_factor =0;
                        break;
                    case 1:
                        speed_offset = 1;
                        speed_factor = 0.5;
                        break;
                    case 2:
                        speed_offset = -1;
                        speed_factor = -0.5;
                        break;
                    default:
                        speed_offset = 0;
                        speed_factor =0;
                        printf("undefined value for PWM level\n");
                        break;
                }
                PWM_value_DC = 15.0 + (speed_factor)*(DC_level) +speed_offset;/*1.0;*/
            }

            switch(canReceivedData.data.bytes[0])
            {
                case 0:
                    speed_offset = 0;
                    speed_factor =0;
                    break;
                case 1:
                    speed_offset = 1;
                    speed_factor = 0.5;
                    break;
                case 2:
                    speed_offset = -1;
                    speed_factor = -0.5;
                    break;
                default:
                    speed_offset = 0;
                    speed_factor =0;
                    printf("undefined value for PWM level\n");
                    break;
            }
            PWM_value_servo = 15.0 + (speed_factor)*(servo_level) +speed_offset; /*2.0;*/

            pwm2.set(PWM_value_DC);
            pwm1.set(PWM_value_servo);
        }
        else
        {
            pwm2.set(neutral);
            pwm1.set(neutral);
            LE.toggle(1);
        }
        return true;
    }

    private:
    PWM pwm2; /* set pw2 for dc motor */
    PWM pwm1; /* set pw2 for dc motor */

};



#endif /* MOTOR_HPP_ */
