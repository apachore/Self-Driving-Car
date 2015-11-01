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


int front_start_time =0,back_start_time=0, right_start_time=0,left_start_time=0;
int front_echo_width = 0,back_echo_width = 0, right_echo_width = 0,left_echo_width = 0;
int front=0, back = 0, right =0, left = 0,start =0;
int start_flag, pin_number;
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
    back_start_time = lpc_timer_get_value(lpc_timer0);
}
void front_rise_edge()
{
    front_start_time = lpc_timer_get_value(lpc_timer0);
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
    static QueueHandle_t sensor_data_q = scheduler_task::getSharedObject("sensor_queue");
    front_echo_width = lpc_timer_get_value(lpc_timer0) - front_start_time;
    front = (front_echo_width*2.54)/147;
     pin_number = 2;
    if (!xQueueSend(sensor_data_q, &pin_number, 0))
    {
        LE.on(1); //   puts("Failed to send item to queue");
    }
}
void back_fall_edge()
{
    static QueueHandle_t sensor_data_q = scheduler_task::getSharedObject("sensor_queue");
    back_echo_width = lpc_timer_get_value(lpc_timer0) - back_start_time;
    back = (back_echo_width * 0.017) - 7;
    pin_number = 4;
    if (!xQueueSend(sensor_data_q, &pin_number, 0))
    {
        LE.on(2); //         puts("Failed to send item to queue");
    }
}
void left_fall_edge()
{
   static QueueHandle_t sensor_data_q = scheduler_task::getSharedObject("sensor_queue");
    left_echo_width = lpc_timer_get_value(lpc_timer0) - left_start_time;
    left = (left_echo_width*2.54)/147;
    pin_number = 6;
    if (!xQueueSend(sensor_data_q, &pin_number, 0))
    {
        LE.on(3); //         puts("Failed to send item to queue");
    }
}
void right_fall_edge()
{
   static QueueHandle_t sensor_data_q = scheduler_task::getSharedObject("sensor_queue");
    right_echo_width = lpc_timer_get_value(lpc_timer0) - right_start_time;
    right =(right_echo_width*2.54)/147;
    pin_number = 0;
    if (!xQueueSend(sensor_data_q, &pin_number, 0))
    {
        LE.on(4); //       puts("Failed to send item to queue");
    }
}


bool sensor_init(void)
{
    QueueHandle_t sensor_data_q = xQueueCreate(15, sizeof(int));
    scheduler_task::addSharedObject("sensor_queue", sensor_data_q);
    const uint8_t port2_1 = 1, port2_3 = 3, port2_5 = 5,port2_7 = 7;

    eint3_enable_port2(port2_1, eint_rising_edge, front_rise_edge);
   //HCSR04 :interrupt for rising edge not required
    eint3_enable_port2(port2_5, eint_rising_edge, left_rise_edge);
    eint3_enable_port2(port2_7, eint_rising_edge, right_rise_edge );

    eint3_enable_port2(port2_1, eint_falling_edge, front_fall_edge);
    eint3_enable_port2(port2_3, eint_falling_edge, back_fall_edge);
    eint3_enable_port2(port2_5, eint_falling_edge, left_fall_edge);
    eint3_enable_port2(port2_7, eint_falling_edge, right_fall_edge);

    LPC_PINCON->PINSEL3 |= (3 << 28); // ADC-4 is on P1.30, select this as ADC0.4

    lpc_timer_enable(lpc_timer0, 1);
    LPC_GPIO2->FIODIR |= (1 << 0);
    LPC_GPIO2->FIODIR |= (1 << 2);
    LPC_GPIO2->FIODIR |= (1 << 4);
    LPC_GPIO2->FIODIR |= (1 << 6);
    can_Tx_Rx_init();
    return true;
//    return (NULL != sensor_data_q);

}
bool sensor_compute()
{
    static QueueHandle_t sensor_data_q = scheduler_task::getSharedObject("sensor_queue");
    if(start_flag == 0)
    {
        pin_number = 0;
        start_flag = 1;
        if (!xQueueSend(sensor_data_q, &pin_number, 0))
        {
           // LE.on(4); //         puts("Failed to send item to queue");
        }
    }
    if (xQueueReceive(sensor_data_q, &pin_number, 0))
    {
        if(pin_number ==2)
        {
            sensor_trig_HCSR04(pin_number);
        }
        else
        {
        sensor_trig_MB1010(pin_number);
        }
    }
  //printf("%d\n",front);

    //can_msg_t msgRx;
    //CAN_rx(can1,&msgRx, 0); //Receive msg over CAN Bus

//    if(msgRx.msg_id == 0x610)
//    {
//        can_Boot_stat();
//    }
//    can_Heart_beat();
    if((front<300)||(left<300)||(right<300)||(back<150))
    {
        can_Tx_Sensor_data();
    }
  if(CAN_is_bus_off(can1))
  {
      CAN_reset_bus(can1);
      LE.toggle(2);
  }
    return true;
}

