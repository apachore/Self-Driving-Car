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
#include "can_comms.hpp"
#include "gpio.hpp"
static int speed_pulse_cnt_sec;
void Speed_Pulse_Count()
{

	extern int speed_pulse_cnt,speed_pulse_cnt_100ms;
	speed_pulse_cnt_sec = speed_pulse_cnt;
	speed_pulse_cnt = 0;
//	printf("\n\nperiodic spd Pul cnt: %d\n",speed_pulse_cnt_sec);
}

MotorTask::MotorTask(uint8_t priority) :
        scheduler_task("MotorTask", 512 * 4, priority), pwm2(PWM::pwm2, 100), pwm1(PWM::pwm1, 100),
        p2_pin6(LPC1758_GPIO_Type::P2_6),p2_pin7(LPC1758_GPIO_Type::P2_7)
{
    count_init = 1;
    count_rev = 1;
//    just = 0;
    setRunDuration(100);
}

bool MotorTask::init(void)
{
    pwm2.set(15);   /*Initialization sequence for DC motor*/
    delay_ms(100);
    pwm1.set(15);
    delay_ms(100);
    p2_pin6.setAsOutput();
    p2_pin7.setAsOutput();
    p2_pin6.setLow();
    p2_pin7.setLow();

    return true;
}

bool MotorTask::run(void *p)
{
//    extern int speed_pulse_cnt;
//    static int speed_pulse_current,speed_pulse_prev;
//	speed_pulse_current = speed_pulse_cnt;
//	printf("\nSpeed_Pulse Count: %d",speed_pulse_current);
//	speed_pulse_cnt = 0;
//	speed_pulse_prev = speed_pulse_current;

	int servo_control, servo_level, DC_control, DC_level;
    float speed_factor, speed_offset, PWM_value_DC, PWM_value_servo, speed_correction;
    static float PWM_value_DC_last, PWM_value_servo_last;
//    static QueueHandle_t receivedDataQueue = scheduler_task::getSharedObject("MasterMotorQueue");
    extern can_msg_t canReceivedData;
    int speed_pls_exp,speed_pls_act,speed_pls_diff;
    static uint32_t speed_miss,speed_miss_cont;
    static int DC_level_last;
    extern int Mstr_Mtr_cmd;

//    printf("\nMstr_Mtr_cmd : %d",Mstr_Mtr_cmd);

    if (count_init)
    {
        config_init();
        count_init--;
    }

    if (/*xQueueReceive(receivedDataQueue, &canReceivedData, 0)*/1)
    {

//        LE.toggle(4);
        servo_control = canReceivedData.data.bytes[0];
        servo_level = canReceivedData.data.bytes[1];
        DC_control = canReceivedData.data.bytes[2];
        DC_level = canReceivedData.data.bytes[3];
        LD.setNumber(DC_control*10 + DC_level);

       // printf("\nDC: Control %x, Level_exp %x",DC_control,DC_level);
//        printf("\nServo: Control %x, Level %x",servo_control,servo_level);

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
            	if(DC_level==DC_level_last)
            	{
                    speed_offset = 0;
                    speed_factor =0;
                    LD.setNumber(10);
            	}
            	else
            	{
            		pwm2.set(15);
            		vTaskDelay(25);
            		pwm2.set(12);
                    vTaskDelay(100);
                    speed_offset = 0;
                    LD.setNumber(13);
                    speed_factor =0;
            	}

            	p2_pin6.setLow();
            	p2_pin7.setLow();

            speed_offset = 0;
            speed_factor =0;
            count_rev = 1;
            break;
            case 1:
            speed_offset = 0.4;
            speed_factor = 0.2;
            count_rev = 1;
            p2_pin6.setHigh();
            p2_pin7.setLow();
            break;
            case 2:
            speed_offset = -0.6;
            speed_factor = -0.2;
            p2_pin6.setLow();
            p2_pin7.setHigh();
            break;
            default:
            speed_offset = 0;
            speed_factor =0;
            count_rev = 1;
            p2_pin6.setLow();
            p2_pin7.setLow();
            break;
        }
        PWM_value_DC = 15 + (speed_factor)*(DC_level) + speed_offset;

        switch(DC_level)
        {
        case 0:
        	speed_pls_exp = 0;
        	break;
        case 1:
        	speed_pls_exp = 8;
        	break;
        case 2:
        	speed_pls_exp = 13;
        	break;
        case 3:
        	speed_pls_exp = 25;
			break;
        default:
        	speed_pls_exp = 0;
        }

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

        speed_pls_diff = speed_pulse_cnt_sec - speed_pls_exp;
        if((-3<=speed_pls_diff) && (speed_pls_diff<=3))
        {
//        	speed_miss--;
//        	speed_miss_cont--;
//        	if(speed_miss<0)
//        	{
        		speed_miss = 0;
        		speed_miss_cont = 0;
//        	}
        }
        else
        {
        	speed_miss++;
        	speed_miss_cont++;
        }
//        speed_miss_cont = 5;

//    	printf("\nspeed_miss  : %d",speed_miss);
//    	printf("\nspeed_miss_cont  : %d",speed_miss_cont);
    	speed_correction = (speed_miss_cont/20)*0.3;

        if(speed_miss > 1)
        {
//        	speed_miss_cont++;
        	if(speed_pulse_cnt_sec < speed_pls_exp)
        	{
//        		printf("\nspeed_miss_cont  : %d",speed_miss_cont);
        		if(DC_control==1)
        			{
        			PWM_value_DC = PWM_value_DC + speed_correction;
//        	        printf("\nDC_value_actual : %f",PWM_value_DC);
        			}
        		else if(DC_control==2)
        		{
        			PWM_value_DC = PWM_value_DC - speed_correction;
        		}
        		else
        		{
        			PWM_value_DC = PWM_value_DC;
        		}

        		speed_miss = 0;
        	}
        	else if(speed_pulse_cnt_sec > speed_pls_exp)
        	{
        		if(DC_control==1)
        			{
        			PWM_value_DC = PWM_value_DC - speed_correction;
        			}
        		else if(DC_control==2)
        		{
        			PWM_value_DC = PWM_value_DC + speed_correction;
        		}
        		else
        		{
        			PWM_value_DC = PWM_value_DC;
        		}

        		speed_miss = 0;
        	}
        	else
        	{
        		PWM_value_DC = PWM_value_DC;
        		speed_miss_cont = 0;
        	}
        }

        else
		{
        	PWM_value_DC = PWM_value_DC;
		}

        pwm1.set(PWM_value_servo);
        pwm2.set(PWM_value_DC);
//        printf("\nDC_value_actual : %f",PWM_value_DC);
//        printf("\nspeed_miss_cont  : %d",speed_miss_cont);

        DC_level_last = DC_level;

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
