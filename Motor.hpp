/*
 * Motor.hpp
 *
 *  Created on: 25-Oct-2015
 *      Author: Su
 */

#ifndef MOTOR_HPP_
#define MOTOR_HPP_
#include "scheduler_task.hpp"



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
