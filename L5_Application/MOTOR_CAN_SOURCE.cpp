/*
 * MOTOR_CAN_SOURCE.cpp
 *
 *  Created on: 26-Oct-2015
 *      Author: Su
 */
#include <stdio.h>
#include "scheduler_task.hpp"
#include "MOTOR_CAN_HEADER.hpp"
#include "utilities.h"
#include "io.hpp"
#include "lpc_pwm.hpp"
#include "can.h"
#include "file_logger.h"
#include "string.h"
#include "stdlib.h"



QueueHandle_t Master_Motor_q;
int final_dist_remaining;
float gps_longitude;
float gps_latitude;
uint8_t sensor_data[4];

void BusOffCb(uint32_t param)
{
    printf("BusOffCb : Error");
}

void DataOverCanBuffer(uint32_t param)
{
    printf("DataOverCanBuffer : Error");
}

void Transmit_init()
{

}

void Receive_init()
{
    CAN_init(can1, 100, 100, 100, *BusOffCb,*DataOverCanBuffer);/*Initializing CAN bus*/
    CAN_reset_bus(can1);          /*Reset CAN bus*/

    const can_std_id_t slist[]  { CAN_gen_sid(can1, 0x210), CAN_gen_sid(can1, 0x220), CAN_gen_sid(can1, 0x240),
            CAN_gen_sid(can1, 0x610), CAN_gen_sid(can1, 0x250), CAN_gen_sid(can1, 0x410)};    /*CAN standard ID list*/

    /*Motor Control message from Master : 0x220 */
    /*Boot Request message from Master : 0x610 */

    Master_Motor_q = xQueueCreate(10, sizeof(can_msg_t));
    scheduler_task::addSharedObject("MasterMotorQueue", Master_Motor_q);

    can_std_grp_id_t *grplist=NULL;
    can_ext_id_t *ext=NULL;
    can_ext_grp_id_t *extgrp=NULL;
    //CAN_bypass_filter_accept_all_msgs();

    CAN_setup_filter(slist,6,grplist,0,ext,0,extgrp,0);  /*CAN message filter */
}

void Transmitter_message()
{
    can_msg_t canMessageBlock,check_0x610,Boot_send;
    canMessageBlock.msg_id = 0x280;
    canMessageBlock.frame_fields.data_len = 8;
    canMessageBlock.data.qword = 0x222; /*Sending Heart beat message data*/
    Boot_send.msg_id = 0x620;
    Boot_send.frame_fields.data_len = 8;
    Boot_send.data.qword = 0x224; /*Sending Boot reply message data*/

    bool flag = CAN_tx(can1,&canMessageBlock, 0);

    if(flag)
    {
        printf("Heart Beat message 0x220 sent successfully \n ");
        LOG_INFO("Heart Beat message  0x220 sent successfully \n");
    }
    else
    {
        LOG_ERROR("Heart Beat message NOT 0x220 sent successfully \n");
        printf("Heart Beat message NOT 0x220 sent successfully \n ");
    }

    if(CAN_rx(can1,&check_0x610,1000))
    {

        printf("Boot request message 0x610 received successfully \n ");
        CAN_tx(can1,&Boot_send,1000);
        printf("Boot response message 0x620 sent successfully \n ");
        LOG_INFO("Boot response message 0x620 sent successfully \n");
    }
    else
    {
       /*do nothing. No need to send any message*/
    }
}



void Receiver_message()
{
    can_msg_t rx_msg;
    if(CAN_rx(can1, &rx_msg, 10))
    {
        LE.on(1);
        switch(rx_msg.msg_id)
        {
            case 0x210:
                sensor_data[0] =rx_msg.data.bytes[0];
                sensor_data[1] =rx_msg.data.bytes[1];
                puts("sensor data received");
                LE.toggle(2);
                sensor_data[2] =rx_msg.data.bytes[2];
                sensor_data[3] =rx_msg.data.bytes[3];
            break;
            case 0x220:
            LE.toggle(3);
            //printf("%x", rx_msg.msg_id);
            //printf("%x", rx_msg.data.bytes[1]);
            if(!xQueueSend(Master_Motor_q, &rx_msg, 0))
            {
               // LE.toggle(2);
              //printf("Queue Full\n");
            }
            else
            {

            }

           break;
            case 0x240:
                memcpy(&gps_latitude,&rx_msg.data.dwords[0],4);
                memcpy(&gps_longitude,&rx_msg.data.dwords[1],4);
               break;
            case 0x250:
                memcpy(&final_dist_remaining,&rx_msg.data.words[0],2);


               break;

           default:
               break;

        }

    }
    else
    {
        puts("No message received\n");
        LE.toggle(4);
    }
    //printf("This is Motor RX function\n");

}


bool CANInitialization()
{
    Receive_init();
    return true;
}




