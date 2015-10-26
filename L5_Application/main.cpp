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
//#include "can.c"
//#include "scheduler_task.cpp"
#include "scheduler_task.hpp"

class hw3 : public scheduler_task
{

private:
GPIO p2_wire0;

public:
hw3(uint8_t priority) :
scheduler_task("hw_task", 2000, priority),p2_wire0(LPC1758_GPIO_Type::P2_0)
{
/* Nothing to init */
}

bool init(void) {

Uart3& u3 = Uart3::getInstance();
u3.init(9600,1,1);
return true;
}

bool run(void *p)
{
char recChar;
Uart3& u3 = Uart3::getInstance();
u3.putChar('R');
u3.getChar(&recChar);
printf("%c\n", recChar);
vTaskDelay(100);
return true;
}
};

#if 0
class hw_Task : public scheduler_task
{

    private:
          GPIO p2_wire0;

    public:
        hw_Task(uint8_t priority) :
        scheduler_task("hw_Task", 2000, priority),p2_wire0(LPC1758_GPIO_Type::P2_0)
        {
        /* Nothing to init */
        }

        bool init(void) {

        Uart3& u3 = Uart3::getInstance();
        u3.init(9600,10,10);
            return true;
        }

        bool run(void *p)
        {   char charout;
            Uart3& u3 = Uart3::getInstance();
            u3.putChar('Y');
           if( u3.getChar(&charout))
           {
               LE.on(3);
               vTaskDelay(100);
           }

            //printf("%c\n",charout);
            //vTaskDelay(100);


            return true;
        }
};
#endif
#if 0
 can_msg_t recmsg;
 bool status;


void A(uint32_t param)
{

}

void B(uint32_t param)
{

}

 class rectask : public scheduler_task

 {
     public:
         rectask(uint8_t priority) :
         scheduler_task("rectask", 2000, priority)
         {
             //setRunDuration(100);
         /* Nothing to init */

         }
         bool init(void) {

             status = CAN_init(can1, 500, 1000, 1000, *A, *B);
             CAN_bypass_filter_accept_all_msgs();
             CAN_reset_bus(can1);
             return true;
         }

         bool run(void *p)

         {
             bool rec;
             rec = CAN_rx(can1, &recmsg,1000);
//             printf("ret val INIT is: %d\n",status);
               //printf("%d",CAN_is_bus_off(can1));
             /*if(rec)
                 LE.toggle(1);
             else
                 LE.toggle(2);*/
             if (recmsg.msg_id==0x2)
             {
                 //if(recmsg.data.qword==0x0000000000000010)
                 //{
                    LE.on(1);
//                    printf("data\n");
                 //}
                 //else if (recmsg.data.qword==0x0000000000000001)
                 //{
                    // LE.off(1);
                 //}
             }
             else
                 LE.off(1);
             //printf("ret val RX is: %d\n",ret);
             return true;
         }
 };
#endif
int main(void)
{
    scheduler_add_task(new hw3(PRIORITY_HIGH));
    //scheduler_add_task(new hw_Task(PRIORITY_HIGH));
    scheduler_start(); ///< This shouldn't return

    //scheduler_add_task(new hw_Task(PRIORITY_HIGH));
       // scheduler_start();
    //scheduler_add_task(new rectask(PRIORITY_HIGH)); //for can
   // while(1)
        //{

       //     printf("%s Temperature:%f Light:%d%%\n","**Welcome to San Jose State University**\n",TS.getFarenheit(),LS.getPercentValue());

     //       delay_ms(2000);
     //   }


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
#if 0
    LPC_GPIO2->FIODIR &= ~(1 << 0);
    LPC_GPIO2->FIODIR |= (1 << 1);
        while(1)
        {
            if(LPC_GPIO2->FIOPIN & (1 << 0))
            {
                LPC_GPIO2->FIOCLR = (1 << 1);
            }
            else
            {
                LPC_GPIO2->FIOSET = (1 << 1);
            }

        }
#endif
    //scheduler_start(); ///< This shouldn't return*/
    /*while(1)
    {int light_value = LS.getRawValue();
            unsigned light_value_percentage=LS.getPercentValue();

    printf("Light:%d Light:%d%%\n",light_value,light_value_percentage);
    //vTaskDelay(500);
    delay_ms(1000);
    }*/

    /*int light_value = LS.getRawValue();
                unsigned light_value_percentage=LS.getPercentValue();

        printf("Light:%d Light:%d%%\n",light_value,light_value_percentage);
        vTaskDelay(1000);
        //delay_ms(1000);*/

    //scheduler_add_task(new LightSensorTask(PRIORITY_HIGH));//(correct)
    //scheduler_start(); ///< This shouldn't return(correct)

    //return -1;

   /*LPC_PINCON->PINSEL4 &= ~(0xF << 16); // Clear values
   LPC_PINCON->PINSEL4 |=  (0xA << 16); // Set values for UART2 Rx/Tx */

    /*Uart2 &u2 = Uart2::getInstance();
    u2.init(9600, 50, 50);
    //u2.printf("Hello World\n");
    //u2.putline("Hello World\n");*/

    //scheduler_add_task(new hw_Task(PRIORITY_HIGH));
    //scheduler_start();


        /*CAN_init(can1,250,10,10,0,0);
        CAN_bypass_filter_accept_all_msgs(void);
        CAN_rx(can1,&msg,100);
        can_msg_t msg;
        msg.frame_fields.is_29bit = 1;
        msg.frame_fields.data_len = 8;       // Send 8 bytes
        msg.data.qword = 0x1122334455667788; // Write all 8 bytes of data at once
        //CAN_tx(can_1, &msg, portMAX_DELAY);*/
        //scheduler_start(); ///< This shouldn't return

        return -1;

}
