/*
 * IOmodule.cpp

 *
 *  Created on: 26-Oct-2015
 *      Author: User
 */

#include "IOmodule.hpp"

/*
 *
 * IO task - to print messages on LCD
 *
 * received from sensor and GPS
 *
 *
 */

IOTask::IOTask(uint8_t priority) :
            scheduler_task("IOTask", 2000, priority),
            IO_uart(Uart2 ::getInstance())
        {
                IO_uart.init(9600,io_rx_q_size,io_tx_q_size);        /* initialize uart and set baud rate */
        }

bool IOTask::init(void)
{
        CAN_init(can1,100,64,64,NULL,NULL);              /*initialize can1 */
        ack_msg1.frame_fields.is_29bit = 0;
        ack_msg1.frame_fields.data_len = 0;
        ack_msg1.data.bytes[1]=01;

        can_std_grp_id_t *grplist=NULL;
        can_ext_id_t *ext=NULL;
        can_ext_grp_id_t *extgrp=NULL;

         CAN_setup_filter(CANmsglist,5,grplist,0,ext,0,extgrp,0);
         CAN_reset_bus(can1);              /* reset can1 */
         vTaskDelay(100);                 /* Wait for 100 ms*/
         IO_uart.putline("FO");           /* initialize LCD */
         vTaskDelay(100);
         return true;
}

bool IOTask:: run(void *p)
{
    bool tx_flag,rx_flag;
    rx_flag=CAN_rx(can1, &rx_msg, 1000); /* receive can message */
    if(rx_flag)
    {
        switch(rx_msg.msg_id)
        {
            case 0x610:
                LOG_INFO("Boot request received");
                CAN_tx(can1, &ack_msg1, portMAX_DELAY);
                break;
            case 0x630:
                 LOG_INFO("start controller message from master");
                break;
         }
     }
        return true;
}





