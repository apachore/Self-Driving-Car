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


int front_start_time =0,back_start_time=0, right_start_time=0,left_start_time=0;
int front_echo_width = 0,back_echo_width = 0, right_echo_width = 0,left_echo_width = 0;
int front=0, back = 0, right =0, left = 0,start =0;
int amb_temp;
long yield = 0;
int start_flag, pin_number,count;
void sensor_trig_MB1010(int pin)
{
    LPC_GPIO2->FIOPIN &= ~(1 << pin);
    LPC_GPIO2->FIOPIN |= (1 << pin);
    delay_us(20);
    LPC_GPIO2->FIOPIN &= ~(1 << pin);
}
void sensor_trig_HCSR04(int pin)
{
    LPC_GPIO2->FIOPIN &= ~(1 << pin);
    LPC_GPIO2->FIOPIN |= (1 << pin);
    delay_us(10);
    LPC_GPIO2->FIOPIN &= ~(1 << pin);
    left_start_time = lpc_timer_get_value(lpc_timer0);
}
void sensor_trig_HCSR04b(int pin)
{
    LPC_GPIO2->FIOPIN &= ~(1 << pin);
    LPC_GPIO2->FIOPIN |= (1 << pin);
    delay_us(10);
    LPC_GPIO2->FIOPIN &= ~(1 << pin);
    back_start_time = lpc_timer_get_value(lpc_timer0);
}
void front_rise_edge()
{
    front_start_time = lpc_timer_get_value(lpc_timer0);
}
void back_rise_edge()
{
    back_start_time = lpc_timer_get_value(lpc_timer0);
}
void left_rise_edge()
{
    left_start_time = lpc_timer_get_value(lpc_timer0);
}
void right_rise_edge()
{
    right_start_time = lpc_timer_get_value(lpc_timer0);
}
void front_fall_edge()
{
    front_echo_width = lpc_timer_get_value(lpc_timer0) - front_start_time;
    front = (front_echo_width*2.54)/147;
    portYIELD_FROM_ISR(yield);
}
void back_fall_edge()
{
    back_echo_width = lpc_timer_get_value(lpc_timer0) - back_start_time;
    back = (back_echo_width * 0.017) - 7;
    portYIELD_FROM_ISR(yield);
}
void left_fall_edge()
{
    left_echo_width = lpc_timer_get_value(lpc_timer0) - left_start_time;
    left = (left_echo_width* 0.017) - 7;
    portYIELD_FROM_ISR(yield);
}
void right_fall_edge()
{
    right_echo_width = lpc_timer_get_value(lpc_timer0) - right_start_time;
    right =(right_echo_width* 0.017) - 7;
    portYIELD_FROM_ISR(yield);

}

void interrupt_init()
{
       const uint8_t port2_1 = 1, port2_3 = 3, port2_5 = 5,port2_7 = 7;
       eint3_enable_port2(port2_1, eint_rising_edge, front_rise_edge);
       eint3_enable_port2(port2_7, eint_rising_edge, right_rise_edge );

       eint3_enable_port2(port2_1, eint_falling_edge, front_fall_edge);
       eint3_enable_port2(port2_3, eint_falling_edge, back_fall_edge);
       eint3_enable_port2(port2_5, eint_falling_edge, left_fall_edge);
       eint3_enable_port2(port2_7, eint_falling_edge, right_fall_edge);
}

void config_trigger()
{
    LPC_GPIO2->FIODIR |= (1 << 0);
    LPC_GPIO2->FIODIR |= (1 << 2);
    LPC_GPIO2->FIODIR |= (1 << 4);
    LPC_GPIO2->FIODIR |= (1 << 6);
}
bool sensor_init(void)
{

    config_trigger();
    interrupt_init();
    lpc_timer_enable(lpc_timer0, 1);
    can_Tx_Rx_init();

    return true;
}
bool sensor_compute()
{
    sensor_trig_MB1010(0); //Trigger first sensor
delay_ms(25);
sensor_trig_MB1010(6);
delay_ms(25);
sensor_trig_HCSR04(4);

delay_ms(25);
sensor_trig_HCSR04b(2);
delay_ms(15);

  //  LE.setAll(0);
    //can_msg_t msgRx;
    //CAN_rx(can1,&msgRx, 0); //Receive msg over CAN Bus

//    if(msgRx.msg_id == 0x610)
//    {
//        can_Boot_stat();
//    }
//    can_Heart_beat();

    printf("%d %d %d %d\n",front, left, right,back);

    LD.setNumber(front/10);
    if(front >255){front =255;}
    if(back>255){back =255;}
    if(right>255){right =255;}
    if(left>255){left=255;}

        can_Tx_Sensor_data();
        front = 0; back = 0; right = 0; left = 0;

  if(CAN_is_bus_off(can1))
  {
      CAN_reset_bus(can1);

  }
    return true;
}

