/*========================================
 ==========================================================================================
 Name        : sensors.cpp
 Author      : Bhupendra Naphade
 Version     : 1.0
 + Description : Program to read Ultrasonic sensors & Calculate Distance
 +=============================================================================================
 */
#include "tasks.hpp"
#include "shared_handles.h"
#include "scheduler_task.hpp"
//RC
#include "sensor.hpp"
#include "eint.h"
//#include "GPIO.hpp"
#include "LED.hpp"
#include "lpc_timers.h"
#include "utilities.h"
#include "lpc_pwm.hpp"
#include "adc0.h"
#include "scheduler_task.hpp"
#include "can.h"
#include "can_Tx_Rx.hpp"
#include "LED_Display.hpp"
#include "sensor_defines.hpp"


int front_start_time =0,back_start_time=0, right_start_time=0,left_start_time=0;
int front_echo_width = 0,back_echo_width = 0, right_echo_width = 0,left_echo_width = 0;
int front=0, back = 0, right =0, left = 0,start =0;
int amb_temp;
long yield = 0;
int start_flag, pin_number,count;
int adc_val;
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
    front = front_echo_width/PW_CM_CONV;
    portYIELD_FROM_ISR(yield);
}
void back_fall_edge()
{
    back_echo_width = lpc_timer_get_value(lpc_timer0) - back_start_time;
    back = back_echo_width/PW_CM_CONV;
    portYIELD_FROM_ISR(yield);
}
void left_fall_edge()
{
    left_echo_width = lpc_timer_get_value(lpc_timer0) - left_start_time;
    left = left_echo_width/PW_CM_CONV;
    portYIELD_FROM_ISR(yield);
}
void right_fall_edge()
{
    right_echo_width = lpc_timer_get_value(lpc_timer0) - right_start_time;
    right =right_echo_width/PW_CM_CONV;
    portYIELD_FROM_ISR(yield);

}

void sensor_distance_limit()
{
    if(front >DISTANCE_LIMIT)
    {
        front = DISTANCE_LIMIT;
    }
    if(back>DISTANCE_LIMIT)
    {
        back = DISTANCE_LIMIT;
    }
    if(right>DISTANCE_LIMIT)
    {
        right = DISTANCE_LIMIT;
    }
    if(left>DISTANCE_LIMIT)
    {
        left = DISTANCE_LIMIT;
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
    LPC_PINCON->PINSEL1 |= (1 << 20); // ADC-3 is on P0.26, select this as ADC0.3
       LPC_PINCON->PINSEL3 |= (3 << 28); // ADC-4 is on P1.30, select this as ADC0.4
       LPC_PINCON->PINSEL3 |= (3 << 30); // ADC-5 is on P1.31, select this as ADC0.5


}
bool sensor_init(void)
{

    config_sensor_pins();
    delay_ms(SENSOR_INIT_DELAY); //Time required by sensor for calibration.
    interrupt_init();
    lpc_timer_enable(lpc_timer0, TIMER_TICK);
    can_Tx_Rx_init();

    return true;
}
bool sensor_compute()
{
    sensor_trig_MB1010(FRONT_SENSOR_RX);            //Trigger front sensor
    sensor_trig_HCSR04b(BACK_SENSOR_RX);            //Trigger back sensor
    delay_ms(SENSOR_TRIG_DELAY_FB);
    sensor_trig_HCSR04r(FRONT_RIGHT_SENSOR_RX);      //Trigger right sensor
    sensor_trig_HCSR04l(FRONT_LEFT_SENSOR_RX);       //Trigger left sensor
    delay_ms(SENSOR_TRIG_DELAY_RL);

  //  LE.setAll(0);
    //can_msg_t msgRx;
    //CAN_rx(can1,&msgRx, 0); //Receive msg over CAN Bus

//    if(msgRx.msg_id == 0x610)
//    {
//        can_Boot_stat();
//    }
//    can_Heart_beat();
    adc_val =adc0_get_reading(4);

#if DEBUG
   // printf("%d %d %d %d\n",front, left, right,back);
#endif
    LD.setNumber(front/LED_DISP_FACTOR);
    printf("%d\n",adc_val);
    sensor_distance_limit();

    can_Tx_Sensor_data();
     //   front = 0; back = 0; right = 0; left = 0;

  if(CAN_is_bus_off(can1))
  {
      CAN_reset_bus(can1);

  }
    return true;
}

