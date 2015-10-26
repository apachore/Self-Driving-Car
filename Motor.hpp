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


void Motor_PWM_control();

void Bus_Off();

void Data_Over_Run();


class Motor_CAN_RX : public scheduler_task
{
    public:
	q_can_rx;
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
    		      rx_msg.frame_fields.is_29bit = 1;
    		      rx_msg.frame_fields.data_len = 8;

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
    MotorTask(uint8_t priority):
scheduler_task("MotorTask", 512*4, priority)
{

/*Nothing to init*/

}
    bool run(void *p)
    {

        PWM pwm2(PWM::pwm2, 100); /* set pw2 for dc motor */
        PWM pwm1(PWM::pwm1, 100); /* set pw1 for servo motor */
        pwm2.set(15);  /* initialization pulse for dc motor */
        pwm1.set(15);  /* initialization pulse for servo motor*/

        while(1)
            {
                pwm2.set(15);
                delay_ms(500);

                pwm2.set(15.5);
                delay_ms(1000);

                pwm2.set(16);
                delay_ms(1500);

                pwm2.set(16.5);
                delay_ms(1500);

                pwm2.set(16);
                delay_ms(1500);

                pwm2.set(15.5);
                delay_ms(1000);

                pwm2.set(15);
                delay_ms(1000);

                delay_ms(2000);



                pwm2.set(14.8);
                delay_ms(28);

                pwm2.set(15); /*Correction for Forward to Reverse transition*/
                delay_ms(28);

                pwm2.set(14.5);
                delay_ms(1000);

                pwm2.set(14);
                delay_ms(1500);

                pwm2.set(13.5);
                delay_ms(1500);

                pwm2.set(14);
                delay_ms(1500);

                pwm2.set(14.5);
                delay_ms(1000);

                pwm2.set(14.8);
                delay_ms(500);

                delay_ms(500);

            }
        return true;

    }
};



#endif /* MOTOR_HPP_ */
