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

int sensor_start_time = 0, sensor1_echo_width = 0, distance1 = 0, sum1 = 0,
        avg1 = 0, count = 1;
int sensor2_start_time = 0, sensor2_echo_width = 0, distance2 = 0, sum2 = 0,
        avg2 = 0;
int sensor3_start_time = 0, sensor3_echo_width = 0, distance3 = 0, sum3 = 0,
        avg3 = 0;
int reading = 0, sum4 = 0, avg4 = 0, pin_number;

void sensor_trig(int pin)
{

    LPC_GPIO2->FIOPIN &= ~(1 << pin);

    LPC_GPIO2->FIOPIN |= (1 << pin);
    delay_us(10);
    LPC_GPIO2->FIOPIN &= ~(1 << pin);
    sensor_start_time = lpc_timer_get_value(lpc_timer0);
}

void Back_IR_read()
{
    static QueueHandle_t sensor_data_q = scheduler_task::getSharedObject("sensor_queue");
    reading = adc0_get_reading(4);
    reading = (4800 * 2.54) / (reading - 20);
    pin_number = 0;
    if (!xQueueSend(sensor_data_q, &pin_number, 0))
    {
        LE.on(4); //         puts("Failed to send item to queue");
    }

}
void sensor1_fall_edge()
{
    static QueueHandle_t sensor_data_q = scheduler_task::getSharedObject("sensor_queue");
    sensor1_echo_width = lpc_timer_get_value(lpc_timer0) - sensor_start_time;
    distance1 = (sensor1_echo_width * 0.017) - 7;
    pin_number = 2;
    if (!xQueueSend(sensor_data_q, &pin_number, 0))
    {
        LE.on(1); //   puts("Failed to send item to queue");
    }
}

void sensor2_fall_edge()
{
    static QueueHandle_t sensor_data_q = scheduler_task::getSharedObject(
            "sensor_queue");
    sensor2_echo_width = lpc_timer_get_value(lpc_timer0) - sensor_start_time;
    distance2 = (sensor2_echo_width * 0.017) - 7;
    pin_number = 4;
    if (!xQueueSend(sensor_data_q, &pin_number, 0))
    {
        LE.on(2); //       puts("Failed to send item to queue");
    }
}
void sensor3_fall_edge()
{
    static QueueHandle_t sensor_data_q = scheduler_task::getSharedObject(
            "sensor_queue");
    sensor3_echo_width = lpc_timer_get_value(lpc_timer0) - sensor_start_time;
    distance3 = (sensor3_echo_width * 0.017) - 7;
    pin_number = 9;
    if (!xQueueSend(sensor_data_q, &pin_number, 0))
    {
        LE.on(3); //         puts("Failed to send item to queue");
    }
}


bool sensorTask_init(void)
{
    QueueHandle_t sensor_data_q = xQueueCreate(10, sizeof(int));

    scheduler_task::addSharedObject("sensor_queue", sensor_data_q);
    const uint8_t port2_1 = 1, port2_3 = 3, port2_5 = 5;
    eint3_enable_port2(port2_1, eint_falling_edge, sensor1_fall_edge);
    eint3_enable_port2(port2_3, eint_falling_edge, sensor2_fall_edge);
    eint3_enable_port2(port2_5, eint_falling_edge, sensor3_fall_edge);
    LPC_PINCON->PINSEL3 |= (3 << 28); // ADC-4 is on P1.30, select this as ADC0.4

    lpc_timer_enable(lpc_timer0, 1);
    LPC_GPIO2->FIODIR |= (1 << 0);
    LPC_GPIO2->FIODIR |= (1 << 2);
    LPC_GPIO2->FIODIR |= (1 << 4);
    can_Tx_Rx_init();
    //create queue
    LE.on(1);


    pin_number = 0;
    if (!xQueueSend(sensor_data_q, &pin_number, 0))
    {
        LE.on(4); //         puts("Failed to send item to queue");
    }
    return (NULL != sensor_data_q);

}
bool sensorTask_run()
{
    static QueueHandle_t sensor_data_q = scheduler_task::getSharedObject(
        "sensor_queue");
    if (xQueueReceive(sensor_data_q, &pin_number, 0))
    { LE.setAll(0);
        //delay_us(mydel);
        if (pin_number != 9)
        {
            sensor_trig(pin_number);
        }
        else
        {
            Back_IR_read();
        }
    }
    else
    {
        LE.setAll(4);
    }
    if(distance1>90)
    {

        distance1 = 90;
    }
    if(distance2>90)
    {
        distance2 = 90;
    }
    if(distance3>90)
    {
        distance3 = 90;
    }
    if(reading>40)
    {
        reading = 40;
    }
//    printf("sensor1 distance = %d\n",distance1);
    printf("sensor2 distance = %d\n",distance2);
//    printf("sensor3 distance = %d\n",distance3);
//    printf("Back Sensor = %d\n",reading);
    can_msg_t msgRx;
    CAN_rx(can1,&msgRx, 10); //Receive msg over CAN Bus

    if(msgRx.msg_id == 0x610)
    {
        can_Boot_stat();
    }
    can_Heart_beat();

    can_Tx_Sensor_data();
//
    //        sum1= sum1+distance1;
//        sum2= sum2+distance2;
//        sum3= sum3+distance3;

//        count++;
//        if (count == 10)
//            {avg1= sum1/10;
//            avg2= sum2/10;
//            avg3= sum3/10;
//            printf("sensor1 distance = %d\n",avg1);
//            printf("sensor2 distance = %d\n",avg2);
//            printf("sensor3 Reading: %d\n", avg3);
//            count=0;
//            sum1 = sum2 = sum3=0;
//            }
    return true;
}

