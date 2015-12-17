/*========================================
 ==========================================================================================
 Name        : sensors.cpp
 Author      : Bhupendra Naphade
 Version     : 1.0
 + Description : Program to read Ultrasonic sensors & Calculate Distance
 +=============================================================================================
 */
#include <can_transmission_reception.h>
#include "tasks.hpp"
#include "shared_handles.h"
#include "scheduler_task.hpp"
#include "sensor.hpp"
#include "eint.h"
#include "LED.hpp"
#include "lpc_timers.h"
#include "utilities.h"
#include "lpc_pwm.hpp"
#include "scheduler_task.hpp"
#include "can.h"
#include "LED_Display.hpp"
#include "sensor_defines.hpp"
#include "can_communication_ids.h"

int front_start_time =0,back_start_time=0, right_start_time=0,left_start_time=0;
int front_echo_width = 0,back_echo_width = 0, right_echo_width = 0,left_echo_width = 0;
long yield = 0;

sensor_data_t sensor_readings;
void sensor_trig_MB1010(int pin)
{
    LPC_GPIO2->FIOPIN &= ~(1 << pin);
    LPC_GPIO2->FIOPIN |= (1 << pin);
    delay_us(SONAR_TRIG_DELAY);
    LPC_GPIO2->FIOPIN &= ~(1 << pin);
}
void sensor_trig_HCSR04l(int pin)
{
    LPC_GPIO2->FIOPIN &= ~(1 << pin);
    LPC_GPIO2->FIOPIN |= (1 << pin);
    delay_us(HCSR04_TRIG_DELAY);
    LPC_GPIO2->FIOPIN &= ~(1 << pin);
    left_start_time = lpc_timer_get_value(lpc_timer0);
}
void sensor_trig_HCSR04r(int pin)
{
    LPC_GPIO2->FIOPIN &= ~(1 << pin);
    LPC_GPIO2->FIOPIN |= (1 << pin);
    delay_us(HCSR04_TRIG_DELAY);
    LPC_GPIO2->FIOPIN &= ~(1 << pin);
    right_start_time = lpc_timer_get_value(lpc_timer0);
}
void sensor_trig_HCSR04b(int pin)
{
    LPC_GPIO2->FIOPIN &= ~(1 << pin);
    LPC_GPIO2->FIOPIN |= (1 << pin);
    delay_us(HCSR04_TRIG_DELAY);
    LPC_GPIO2->FIOPIN &= ~(1 << pin);
    back_start_time = lpc_timer_get_value(lpc_timer0);
}
void front_rise_edge()
{
    front_start_time = lpc_timer_get_value(lpc_timer0);
}
void front_fall_edge()
{
    front_echo_width = lpc_timer_get_value(lpc_timer0) - front_start_time;
    sensor_readings.front = front_echo_width/PW_CM_CONV;
    portYIELD_FROM_ISR(yield);
}
void back_fall_edge()
{
    back_echo_width = lpc_timer_get_value(lpc_timer0) - back_start_time;
    sensor_readings.back = back_echo_width/PW_CM_CONV;
    portYIELD_FROM_ISR(yield);
}
void left_fall_edge()
{
    left_echo_width = lpc_timer_get_value(lpc_timer0) - left_start_time;
    sensor_readings.left = left_echo_width/PW_CM_CONV;
    portYIELD_FROM_ISR(yield);
}
void right_fall_edge()
{
    right_echo_width = lpc_timer_get_value(lpc_timer0) - right_start_time;
    sensor_readings.right =right_echo_width/PW_CM_CONV;
    portYIELD_FROM_ISR(yield);
}

void sensor_distance_limit()
{
    if(sensor_readings.front >DISTANCE_LIMIT)
    {
        sensor_readings.front = DISTANCE_LIMIT;
    }
    if(sensor_readings.back>DISTANCE_LIMIT)
    {
        sensor_readings.back = DISTANCE_LIMIT;
    }
    if(sensor_readings.right>DISTANCE_LIMIT)
    {
        sensor_readings.right = DISTANCE_LIMIT;
    }
    if(sensor_readings.left>DISTANCE_LIMIT)
    {
        sensor_readings.left = DISTANCE_LIMIT;
    }
}
void interrupt_init()
{
       const uint8_t port2_1 = FRONT_SENSOR_PW, port2_3 = BACK_SENSOR_PW, port2_5 = FRONT_LEFT_SENSOR_PW,port2_7 = FRONT_RIGHT_SENSOR_PW;
       eint3_enable_port2(port2_1, eint_rising_edge, front_rise_edge);

       eint3_enable_port2(port2_1, eint_falling_edge, front_fall_edge);
       eint3_enable_port2(port2_3, eint_falling_edge, back_fall_edge);
       eint3_enable_port2(port2_5, eint_falling_edge, left_fall_edge);
       eint3_enable_port2(port2_7, eint_falling_edge, right_fall_edge);
}

void config_sensor_pins()
{
    LPC_GPIO2->FIODIR |= (1 << FRONT_SENSOR_RX);
    LPC_GPIO2->FIODIR |= (1 << BACK_SENSOR_RX);
    LPC_GPIO2->FIODIR |= (1 << FRONT_LEFT_SENSOR_RX);
    LPC_GPIO2->FIODIR |= (1 << FRONT_RIGHT_SENSOR_RX);
}
bool sensor_init(void)
{

    config_sensor_pins();
    delay_ms(SENSOR_INIT_DELAY);                    //Time required by sensor for calibration.
    interrupt_init();                               //Initialize interrupts for reading sensor pins
    lpc_timer_enable(lpc_timer0, TIMER_TICK);       //Enable timer with tick 1us
    can_Tx_Rx_init();                               //Initialize CAN bus
    return true;
}
bool sensor_compute()
{
    LE.toggle(3);
    sensor_trig_MB1010(FRONT_SENSOR_RX);            //Trigger front sensor
    sensor_trig_HCSR04b(BACK_SENSOR_RX);            //Trigger back sensor
    delay_ms(SENSOR_TRIG_DELAY_FB);
    sensor_trig_HCSR04r(FRONT_RIGHT_SENSOR_RX);     //Trigger right sensor
    sensor_trig_HCSR04l(FRONT_LEFT_SENSOR_RX);      //Trigger left sensor
    delay_ms(SENSOR_TRIG_DELAY_RL);

#if DEBUG
    printf("%d %d %d %d\n",sensor_readings.front, sensor_readings.left, sensor_readings.right,sensor_readings.back);
#endif
    LD.setNumber(sensor_readings.front/LED_DISP_FACTOR);

    sensor_distance_limit();

    //can_Tx_Sensor_data();
   CANTransmit(TSensorData,(uint8_t*)&sensor_readings,sizeof(sensor_data_t));

    return true;
}

