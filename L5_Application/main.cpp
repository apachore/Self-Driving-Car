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
 *          FreeRTOS and stdio printf is pre-configured to use uart0_min.h before main() enters.
 *          @see L0_LowLevel/lpc_sys.h if you wish to override printf/scanf functions.
 *
 */
#include <source/bluetooth.hpp>
#include <source/bluetooth.hpp>
#include "tasks.hpp"
#include "examples/examples.hpp"
#include <stdio.h>
#include "utilities.h"
#include "io.hpp"
#include "eint.h"
#include "gpio.hpp"
#include "LPC17xx.h"
#include"light_sensor.hpp"
#include "uart2.hpp"
#include "ff.h"
#include "uart0.hpp"
#include "uart0_min.h"
#include "uart3.hpp"
#include "uart_dev.hpp"
#include "can.h"
#include "scheduler_task.hpp"

#include "source/can_transmission_reception.h"
#include "source/bluetooth.hpp"


int main(void)
{
    CANInitialization();

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
    scheduler_add_task(new terminalTask(PRIORITY_HIGH));

    /* Consumes very little CPU, but need highest priority to handle mesh network ACKs */
    scheduler_add_task(new wirelessTask(PRIORITY_CRITICAL));

    /* The task for the IR receiver */
    scheduler_add_task(new remoteTask(PRIORITY_LOW));

    /* Your tasks should probably used PRIORITY_MEDIUM or PRIORITY_LOW because you want the terminal
     * task to always be responsive so you can poke around in case something goes wrong.
     */

    /**
     * This is a the board demonstration task that can be used to test the board.
     * This also shows you how to send a wireless packets to other boards.
     */
    //#if 0
        //scheduler_add_task(new example_io_demo());
    //#endif

    /**
     * Change "#if 0" to "#if 1" to enable examples.
     * Try these examples one at a time.
     */
    //#if 0
        //scheduler_add_task(new example_task());
        //scheduler_add_task(new example_alarm());
        //scheduler_add_task(new example_logger_qset());
        //scheduler_add_task(new example_nv_vars());
    //#endif

    /**
     * Try the rx / tx tasks together to see how they queue data to each other.
     */
    //#if 0
        //scheduler_add_task(new queue_tx());
        //scheduler_add_task(new queue_rx());
    //#endif

    /**
     * Another example of shared handles and producer/consumer using a queue.
     * In this example, producer will produce as fast as the consumer can consume.
     */
    //#if 0
        //scheduler_add_task(new producer());
        //scheduler_add_task(new consumer());
    //#endif

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
    //#if 0
       // Uart3 &u3 = Uart3::getInstance();
        //u3.init(WIFI_BAUD_RATE, WIFI_RXQ_SIZE, WIFI_TXQ_SIZE);
        //scheduler_add_task(new wifiTask(Uart3::getInstance(), PRIORITY_LOW));
    //#endif
   // GPIO p2_wire0;
   // p2_wire0.setAsInput();

        //scheduler_start(); ///< This shouldn't return
        //scheduler_add_task(new CanBus(PRIORITY_MEDIUM)); //for can

    scheduler_add_task(new bluetoothTask(PRIORITY_HIGH));

    //scheduler_add_task(new hw_Task(PRIORITY_HIGH));
         scheduler_start(); ///< This shouldn't return
        return -1;

}
