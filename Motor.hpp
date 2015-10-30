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


void Motor_PWM_control();

void Bus_Off();

void Data_Over_Run();


class Motor_CAN_RX : public scheduler_task
{
    public:
	Motor_CAN_RX(uint8_t priority):
scheduler_task("MotorTask", 512*4, priority)
{

		        CAN_init(can1,500,100,100,NULL,NULL);
		        CAN_bypass_filter_accept_all_msgs();
		        CAN_reset_bus(can1);
		        Master_Motor_q = xQueueCreate(1,sizeof(can_data_t));
		        addSharedObject("Motor_Control_q",Master_Motor_q);

}
    bool run(void *p)
    {

    		      can_msg_t rx_msg;
//    		      rx_msg.frame_fields.is_29bit = 1;
//    		      rx_msg.frame_fields.data_len = 8;

    		      if(CAN_rx(can1, &rx_msg, 1000))
    		      {
    		      if(rx_msg.msg_id == 0x220)
    		      {
    		    	    if(! xQueueSend(Master_Motor_q, &rx_msg.data, 0)) {
    		    	    	printf("Queue Full\n");

    		    	    }


    		      }
    		      else
    		      {
                     /*Will be updated with other IO related messages */
    		      }
    		      }
    		      else
    			  {
    		    	  printf("No message received\n");
    			  }

    		printf("This is Motor RX function\n");


        return true;

    }

    private:
    QueueHandle_t Master_Motor_q;

};


class MotorTask : public scheduler_task
{
    public:
    MotorTask(uint8_t priority):scheduler_task("MotorTask", 512*4, priority), pwm2(PWM::pwm2, 100), pwm1(PWM::pwm1, 100)
{
        setRunDuration(100);
/*Nothing to init*/

}
    bool init(void)
    {

      return true;
    }
    bool run(void *p)
    {
        int servo_control,servo_level,DC_control,DC_level;
        float speed_factor, speed_offset, PWM_value_DC,PWM_value_servo;
        static int count = 0;
        static QueueHandle_t receivedDataQueue = scheduler_task::getSharedObject("MasterMotorQueue");
        can_msg_t canReceivedData;

        if (xQueueReceive(receivedDataQueue, &canReceivedData, 0))
        {
            LE.toggle(4);
//            for(i=0;i<4;i++)
//            {
            printf("\nReceived data : %x,%x\n", canReceivedData.data.bytes[1],canReceivedData.data.bytes[3]);
//            }
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
            //printf("DC value: %f\nServo value: %f\n",PWM_value_DC,PWM_value_servo);
            //printf("Queue Received\n");
        }
        else
        {
            LE.toggle(1);
        }
        return true;
    }

    private:
    //QueueHandle_t receivedDataQueue;
    PWM pwm2; /* set pw2 for dc motor */
    PWM pwm1; /* set pw2 for dc motor */
};



#endif /* MOTOR_HPP_ */
