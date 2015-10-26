/*
 * Motor_PWM.cpp
 *
 *  Created on: 25-Oct-2015
 *      Author: Su
 */

#include <stdio.h>
#include "Motor.hpp"
#include "lpc_pwm.hpp"

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
	/*CAN data bytes received from Master to control motor. Will be replaced soon*/
	int byte0,byte1,byte2,byte3;
    PWM pwm2(PWM::pwm2, 100); /* set pw2 for dc motor */
    PWM pwm1(PWM::pwm1, 100); /* set pw1 for servo motor */
	if(Motor_ini == 0)
{
    pwm2.set(15);  /* initialization pulse for dc motor */
    Motor_ini = 1;
}

	else
	{
			switch(byte1)
			{

			case 0 :
				pwm1.set(15);
				break;

			case 1 :
				if(byte0 == 1)
				{
				    pwm1.set(14);
				}
				else if(byte0 == 2)
				{
					pwm1.set(16);
				}
				else
				{
					pwm1.set(15);
				}
				break;

			case 2 :
				if(byte0 == 1)
				{
				    pwm1.set(13);
				}
				else if(byte0 == 2)
				{
					pwm1.set(17);
				}
				else
				{
					pwm1.set(15);
				}
				break;

			default :
				pwm1.set(15);

			}


			switch(byte3)
			{

			case 0 :
				pwm2.set(15);
				break;

			case 1 :
				if(byte2 == 1)
				{
				    pwm2.set(16);
				}
				else if(byte2 == 2)
				{
					pwm2.set(14);
				}
				else
				{
					pwm2.set(15);
				}
				break;

			case 2 :
				if(byte2 == 1)
				{
				    pwm2.set(16.4);
				}
				else if(byte2 == 2)
				{
					pwm2.set(13.6);
				}
				else
				{
					pwm2.set(15);
				}
				break;

			case 3 :
				if(byte2 == 1)
				{
				    pwm2.set(16.8);
				}
				else if(byte2 == 2)
				{
					pwm2.set(13.2);
				}
				else
				{
					pwm2.set(15);
				}
				break;

			default :
				pwm1.set(15);
	}

}
}



