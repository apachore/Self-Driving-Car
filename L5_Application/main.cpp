/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */

/**
 * @file
 * @brief This is the application entry point.
 *             FreeRTOS and stdio printf is pre-configured to use uart0_min.h before main() enters.
 *             @see L0_LowLevel/lpc_sys.h if you wish to override printf/scanf functions.
 *
 */
#include "tasks.hpp"
#include "examples/examples.hpp"

//RC
#include "eint.h"
#include "GPIO.hpp"
#include "LED.hpp"
#include "lpc_timers.h"
#include "utilities.h"
#include "lpc_pwm.hpp"
#include "adc0.h"
volatile int sensor_start_time = 0,sensor1_echo_width = 0, distance1 = 0, sum1 = 0, avg1 = 0,count=1;
volatile int sensor2_start_time = 0,sensor2_echo_width = 0, distance2 = 0, sum2 = 0, avg2 = 0;
volatile int sensor3_start_time = 0,sensor3_echo_width = 0, distance3 = 0, sum3 = 0, avg3 = 0;
int reading = 0,sum4 = 0, avg4= 0;
#define mydel 250000
void sensor_trig(int pin_number)
{
    LPC_GPIO2->FIOPIN &= ~(1 << pin_number);
      LPC_GPIO2->FIOPIN |= (1 << pin_number);
      delay_us(10);
      LPC_GPIO2->FIOPIN &= ~(1 << pin_number);
      sensor_start_time = lpc_timer_get_value(lpc_timer0);
}

void Back_IR_read()
{
    reading = adc0_get_reading(4);
    printf("IR reading:%d\n",reading);
}
void sensor1_fall_edge()
{
        sensor1_echo_width = lpc_timer_get_value(lpc_timer0)-sensor_start_time;
        distance1 =  (sensor1_echo_width* 0.017) - 7;
        sensor_trig(2);
}

void sensor2_fall_edge()
{
    sensor2_echo_width = lpc_timer_get_value(lpc_timer0)-sensor_start_time;
        distance2 =  (sensor2_echo_width* 0.017) - 7;
        sensor_trig(4);
}
void sensor3_fall_edge()
{
    sensor3_echo_width = lpc_timer_get_value(lpc_timer0)-sensor_start_time;
        distance3 =  (sensor3_echo_width* 0.017) - 7;
        sensor_trig(0);
}



class sensorTask : public scheduler_task
{
    private:

    public:
        sensorTask(uint8_t priority) : scheduler_task("sensorTask", 2000, priority)
    {

    }
    bool init(void)
    {
        const uint8_t port2_1 = 1,port2_3 =3,port2_5 = 5;
        eint3_enable_port2(port2_1, eint_falling_edge, sensor1_fall_edge);
        eint3_enable_port2(port2_3, eint_falling_edge, sensor2_fall_edge);
        eint3_enable_port2(port2_5, eint_falling_edge, sensor3_fall_edge);
        LPC_PINCON->PINSEL3 |=  (3 << 28); // ADC-4 is on P1.30, select this as ADC0.4

        lpc_timer_enable(lpc_timer0,1);
        LPC_GPIO2->FIODIR |=(1<<0);
        LPC_GPIO2->FIODIR |=(1<<2);
        LPC_GPIO2->FIODIR |=(1<<4);
       // Back_IR_read();
        sensor_trig(1);
        return true;
    }
    bool run(void *p)
    {

    printf("sensor1 distance = %d\n",distance1);

        delay_us(mydel);

       printf("sensor2 distance = %d\n",distance2);

        delay_us(mydel);
                printf("sensor3 distance = %d\n",distance3);
                delay_us(mydel);
//        sum1= sum1+distance1;
//        sum2= sum2+distance2;
//        sum3= sum3+distance3;
                reading = adc0_get_reading(4);
            //    printf("IR reading:%d\n",reading);

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
};
/**
 * The main() creates tasks or "threads".  See the documentation of scheduler_task class at scheduler_task.hpp
 * for details.  There is a very simple example towards the beginning of this class's declaration.
 *
 * @warning SPI #1 bus usage notes (interfaced to SD & Flash):
 *      - You can read/write files from multiple tasks because it automatically goes through SPI semaphore.
 *      - If you are going to use the SPI Bus in a FreeRTOS task, you need to use the API at L4_IO/fat/spi_sem.h
 *
 * @warning SPI #0 usage notes (Nordic wireless)
 *      - This bus is more tricky to use because if FreeRTOS is not running, the RIT interrupt may use the bus.
 *      - If FreeRTOS is running, then wireless task may use it.
 *        In either case, you should avoid using this bus or interfacing to external components because
 *        there is no semaphore configured for this bus and it should be used exclusively by nordic wireless.
 */
int main(void)
{

    /**
     * A few basic tasks for this bare-bone system :
     *      1.  Terminal task provides gateway to interact with the board through UART terminal.
     *      2.  Remote task allows you to use remote control to interact with the board.
     *      3.  Wireless task responsible to receive, retry, and handle mesh network.
     *
     * Disable remote task if you are not using it.  Also, it needs SYS_CFG_ENABLE_TLM
     * such that it can save remote control codes to non-volatile memory.  IR remote
     * control codes can be learned by typing the "learn" terminal command.
     */

    //             reading = adc0_get_reading(4); // Read current value of ADC-4
    //   reading 4800/(adc0_get_reading(4)- 20))/2.54);
    //             printf("\nADC Reading: %d", reading);

    scheduler_add_task(new terminalTask(PRIORITY_HIGH));

    /* Consumes very little CPU, but need highest priority to handle mesh network ACKs */
    scheduler_add_task(new wirelessTask(PRIORITY_CRITICAL));

    #if 1
    scheduler_add_task(new sensorTask(PRIORITY_CRITICAL));
    #endif
    /* Change "#if 0" t "#if 1" to runCAN msg task*/
    #if 0
    scheduler_add_task(new CANTask(PRIORITY_CRITICAL));
    #endif
    /* Change "#if 0" t "#if 1" to run period tasks; @see period_callbacks.cpp */
    #if 0
    scheduler_add_task(new periodicSchedulerTask());
    #endif

    /* The task for the IR receiver */
    // scheduler_add_task(new remoteTask  (PRIORITY_LOW));

    /* Your tasks should probably used PRIORITY_MEDIUM or PRIORITY_LOW because you want the terminal
     * task to always be responsive so you can poke around in case something goes wrong.
     */

    /**
     * This is a the board demonstration task that can be used to test the board.
     * This also shows you how to send a wireless packets to other boards.
     */
    #if 0
        scheduler_add_task(new example_io_demo());
    #endif

    /**
     * Change "#if 0" to "#if 1" to enable examples.
     * Try these examples one at a time.
     */
    #if 0
        scheduler_add_task(new example_task());
        scheduler_add_task(new example_alarm());
        scheduler_add_task(new example_logger_qset());
        scheduler_add_task(new example_nv_vars());
    #endif

    /**
     * Try the rx / tx tasks together to see how they queue data to each other.
     */
    #if 0
        scheduler_add_task(new queue_tx());
        scheduler_add_task(new queue_rx());
    #endif

    /**
     * Another example of shared handles and producer/consumer using a queue.
     * In this example, producer will produce as fast as the consumer can consume.
     */
    #if 0
        scheduler_add_task(new producer());
        scheduler_add_task(new consumer());
    #endif

    /**
     * If you have RN-XV on your board, you can connect to Wifi using this task.
     * This does two things for us:
     *   1.  The task allows us to perform HTTP web requests (@see wifiTask)
     *   2.  Terminal task can accept commands from TCP/IP through Wifly module.
     *
     * To add terminal command channel, add this at terminal.cpp :: taskEntry() function:
     * @code
     *     // Assuming Wifly is on Uart3
     *     addCommandChannel(Uart3::getInstance(), false);
     * @endcode
     */
    #if 0
        Uart3 &u3 = Uart3::getInstance();
        u3.init(WIFI_BAUD_RATE, WIFI_RXQ_SIZE, WIFI_TXQ_SIZE);
        scheduler_add_task(new wifiTask(Uart3::getInstance(), PRIORITY_LOW));
    #endif

    scheduler_start(); ///< This shouldn't return
    return -1;
}
