/*
 * Motor.hpp
 *
 *  Created on: 25-Oct-2015
 *      Author: Su
 */

#ifndef MOTOR_HPP_
#define MOTOR_HPP_

#include "scheduler_task.hpp"
#include "lpc_pwm.hpp"



void Bus_Off();
void Data_Over_Run();

class MotorTask: public scheduler_task
{
    public:
        MotorTask(uint8_t priority);
        bool init(void);
        bool run(void *p);
        void config_init(void);
        void set_rev(void);

    private:
        PWM pwm2; /* set pw2 for dc motor */
        PWM pwm1; /* set pw1 for servo motor */
        int count_init;
        int count_rev;
        int just;
};

#endif /* MOTOR_HPP_ */
