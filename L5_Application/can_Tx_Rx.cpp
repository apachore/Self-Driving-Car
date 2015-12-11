/*
 * can_Tx_Rx.cpp
 *
 *  Created on: Oct 27, 2015
 *      Author: Bhupe
 */
#include "tasks.hpp"
#include "scheduler_task.hpp"
#include "can_Tx_Rx.hpp"
#include "sensor.hpp"
#include "can_communication_ids.h"

extern int front, back, right, left;
void can_Tx_Rx_init()
{
    CAN_init(can1, 100, 30, 30, NULL,NULL);

    can_std_grp_id_t *grplist=NULL;
      can_ext_id_t *ext=NULL;
      can_ext_grp_id_t *extgrp=NULL;
      //CAN_bypass_filter_accept_all_msgs();
    if( CAN_setup_filter(canMessagesFilterList,4,grplist,0,ext,0,extgrp,0))  /*CAN message filter */
            CAN_reset_bus(can1); //enable the can bus
}


void can_Tx_Sensor_data()
{
    can_msg_t msgTx;
   msgTx.msg_id = TSensorData;
   msgTx.frame_fields.is_29bit = 0;
   msgTx.frame_fields.data_len = 4;       // Send 8 bytes
//   sensor_data_t *p (sensor_data_t*)&msgTx.data.bytes[0];
//   sensor_data_t *p (sensor_data_t*)&msgTx.data.bytes[1];
//   sensor_data_t *p (sensor_data_t*)&msgTx.data.bytes[2];
//   sensor_data_t *p (sensor_data_t*)&msgTx.data.bytes[3];
   msgTx.data.bytes[0] =front;
   msgTx.data.bytes[1] =left;
   msgTx.data.bytes[2] =right;
   msgTx.data.bytes[3] =back;
   if(!CAN_tx(can1, &msgTx, 0))
   {
        LE.off(4) ; //printf("Sensor data not sent");
   }
   else
   {
       LE.on(4);
   }
}
void can_Heart_beat()
{
    can_msg_t msgTx;
   msgTx.msg_id = THeartbeatToMaster;
   msgTx.frame_fields.is_29bit = 0;
   msgTx.frame_fields.data_len = 1;       // Send bytes
   msgTx.data.qword = 0x00;
   if( !(CAN_tx(can1, &msgTx, 0)))
   {
      // LE.toggle(1) ;// printf("Heart beat not sent");
   }
}

void can_Boot_stat()
{
    can_msg_t msgTx;
   msgTx.msg_id = TBootReplyToMaster;
   msgTx.frame_fields.is_29bit = 0;
   msgTx.frame_fields.data_len = 1;       // Send bytes
   msgTx.data.qword = 20151207;
   if( !(CAN_tx(can1, &msgTx, 0)))
   {
       LE.toggle(1) ;// printf("Boot status not sent");
   }
}

void CANReception()
{
    can_msg_t canMessageBlock;

    while (CAN_rx(can1, &canMessageBlock, 0))
        {
            switch (canMessageBlock.msg_id)
            {
                case RKillMessage   :        // Kill Message
                    // Reboot the system if Kill message is received from master
                                   sys_reboot();
                                    break;
                case RBootRequestFromMaster :      // Boot status enquire from Master
//                    if (BootReplySent == 0)
//                        if (BootReplySent == 0)
//                        {
//                            uint32_t bootData = BOOTREPLYDATA;
//                            CANTransmit(TBootReplyToMaster,(uint8_t*)&bootData,sizeof(bootData));
//                            //CANTransmitBootReply();
//                            BootReplySent = 1;
//                        }

                    break;
                case RBootStatusFromMaster  :
                    break;
            }
}}
