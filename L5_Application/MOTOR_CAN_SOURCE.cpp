/*
/ * MOTOR_CAN_SOURCE.cpp
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
#include "can_comms.hpp"
#include "lpc_sys.h"



Can_msg Message;
coordinates checkpoint;

//Label to indicate Boot request from Master
int Boot_req;

void BusOffCb(uint32_t param)
{
    printf("BusOffCb : Error");
}

void DataOverCanBuffer(uint32_t param)
{
    printf("DataOverCanBuffer : Error");
}

//The CAN bus is initialized here and acceptance filter is set for CAN reception
void Receive_init()
{
    CAN_init(can1, 100, 100, 100, *BusOffCb,*DataOverCanBuffer);/*Initializing CAN bus*/
    CAN_reset_bus(can1);          /*Reset CAN bus*/

    const can_std_id_t slist[]  {CAN_gen_sid(can1, 0x020),CAN_gen_sid(can1, 0x210),CAN_gen_sid(can1, 0x220),
            CAN_gen_sid(can1, 0x240), CAN_gen_sid(can1, 0x250),CAN_gen_sid(can1, 0x260),
          CAN_gen_sid(can1, 0x410), CAN_gen_sid(can1, 0x610)};    /*CAN standard ID list*/

    /*Motor Control message from Master : 0x220 */
    /*Boot Request message from Master : 0x610 */

    can_std_grp_id_t *grplist=NULL;
    can_ext_id_t *ext=NULL;
    can_ext_grp_id_t *extgrp=NULL;
    //CAN_bypass_filter_accept_all_msgs();

    CAN_setup_filter(slist,8,grplist,0,ext,0,extgrp,0);  /*CAN message filter */
}

//This function transmits the CAN messages(Ex: Heart beat) which need to be sent once every second
void Transmitter_message_1Hz()
{
    can_msg_t canMessageBlock;
    canMessageBlock.msg_id = 0x280;
    canMessageBlock.frame_fields.data_len = 8;
    canMessageBlock.data.qword = 0x222; /*Sending Heart beat message data*/
    bool flag = CAN_tx(can1,&canMessageBlock, 0);
    if(flag)
    {
//        printf("Heart Beat message 0x220 sent successfully \n ");
        LOG_INFO("Heart Beat message  0x220 sent successfully \n");

        //LED 3 toggled to indicate that Heart Beat message is sent
        LE.toggle(3);
    }
    else
    {
        LOG_ERROR("Heart Beat message NOT 0x220 sent successfully \n");
//        printf("Heart Beat message NOT 0x220 sent successfully \n ");
    }
}

//This function transmits the CAN messages at 10Hz rate
void Transmitter_message()
{
    can_msg_t check_0x610,Boot_send;
    Boot_send.msg_id = 0x620;
    Boot_send.frame_fields.data_len = 8;
    Boot_send.data.qword = 0x224; /*Sending Boot reply message data*/
    if(Boot_req)
    {
//        printf("Boot request message 0x610 received successfully \n ");
        CAN_tx(can1,&Boot_send,1000);
//        printf("Boot response message 0x620 sent successfully \n ");
        LOG_INFO("Boot response message 0x620 sent successfully \n");
        Boot_req = 0;
    }
    else
    {
       /*do nothing. No need to send any message*/
    }
}

int CAN_Receive;
can_msg_t canReceivedData;

//This function receives the CAN messages at 100Hz and copies the required data
void Receiver_message()
{
    can_msg_t rx_msg;
    while(CAN_rx(can1, &rx_msg, 0))
    {
    	CAN_Receive = 0;
        switch(rx_msg.msg_id)
        {
            //Boot request message from Master
            case 0x020:
                sys_reboot();

            //Sensor data from Sensor controller
            case 0x210:
                Message.canReceivedMsg_Sensor=rx_msg;
            break;

            //Motor control command from Master
            case 0x220:
                LE.toggle(2);
                canReceivedData = rx_msg;
            break;

           //Latitude and Longitude information from GPS
            case 0x240:
                memcpy(&checkpoint,&rx_msg.data,sizeof(coordinates));
            break;

           //Distance remaining from GPS
            case 0x250:
                Message.canReceivedMsg_gps2=rx_msg;
              break;

            case 0x260:
                Message.canReceivedMsg_compass=rx_msg;
                break;

            //Boot request from Master
            case 0x610:
                Boot_req =1;
               break;

           default:
               break;

        }

    }

}


bool CANInitialization()
{
    Receive_init();
    return true;
}




