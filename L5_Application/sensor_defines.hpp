/*========================================
 ==========================================================================================
 Name        : sensor_defines.hpp
 Author      : Bhupendra Naphade
 Version     : 1.0
 Description : Sesnor Definitions
 +=============================================================================================
 */

#ifndef L5_APPLICATION_SENSOR_DEFINES_HPP_
#define L5_APPLICATION_SENSOR_DEFINES_HPP_

#define DEBUG                                   0       // Set 1 enable print else 0
#define PW_CM_CONV                              58      //Division factor to convert distance in cm
#define LED_DISP_FACTOR                         10      //Factor for display of front sensor distance on LED display
#define DISTANCE_LIMIT                          255     //Limit sesnor distance
#define SENSOR_THRESH                           10      //Threshold value for deviation in successive reading

#define TIMER_TICK                              1       // 1us per tick for timer
#define SENSOR_INIT_DELAY                       250     //Time required by sensor for initial calibration
#define SONAR_TRIG_DELAY                        20      //Trigger pulse width for MB1010 sensor
#define HCSR04_TRIG_DELAY                       10      //Trigger pulse width for HC SR-04
#define SENSOR_TRIG_DELAY_FB                    55      //Delay to get reading from MB1010 sensor
#define SENSOR_TRIG_DELAY_RL                    30      //Delay to get reading from HCSR-04 sensor


//Sensor trigger and echo pins. RX indicate trigger pin while PW is PWM/Echo
#define FRONT_SENSOR_RX                         0
#define FRONT_SENSOR_PW                         1
#define BACK_SENSOR_RX                          2
#define BACK_SENSOR_PW                          3
#define FRONT_LEFT_SENSOR_RX                    4
#define FRONT_LEFT_SENSOR_PW                    5
#define FRONT_RIGHT_SENSOR_RX                   6
#define FRONT_RIGHT_SENSOR_PW                   7

#define BOOTREPLYDATA                           20150607 //Bootreply data to be transmitted as Boot to master



#endif /* L5_APPLICATION_SENSOR_DEFINES_HPP_ */
