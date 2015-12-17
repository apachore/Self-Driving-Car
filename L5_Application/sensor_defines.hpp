/*
 * sensor_defines.hpp
 *
 *  Created on: Dec 5, 2015
 *      Author: Bhupi
 */

#ifndef L5_APPLICATION_SENSOR_DEFINES_HPP_
#define L5_APPLICATION_SENSOR_DEFINES_HPP_

#define DEBUG                                   1
#define PW_CM_CONV                              58
#define LED_DISP_FACTOR                         10
#define DISTANCE_LIMIT                          255
#define BOOTREPLYDATA                           20151207

#define TIMER_TICK                              1       // 1us per tick
#define SENSOR_INIT_DELAY                       250
#define SONAR_TRIG_DELAY                        20
#define HCSR04_TRIG_DELAY                       10
#define SENSOR_TRIG_DELAY_FB                    55      //Delay to get reading from sonar sensor
#define SENSOR_TRIG_DELAY_RL                    30


//Sensor trigger and echo pins. RX indicate trigger pin while PW is PWM/Echo
#define FRONT_SENSOR_RX                         0
#define FRONT_SENSOR_PW                         1
#define BACK_SENSOR_RX                          2
#define BACK_SENSOR_PW                          3
#define FRONT_LEFT_SENSOR_RX                    4
#define FRONT_LEFT_SENSOR_PW                    5
#define FRONT_RIGHT_SENSOR_RX                   6
#define FRONT_RIGHT_SENSOR_PW                   7








#endif /* L5_APPLICATION_SENSOR_DEFINES_HPP_ */
