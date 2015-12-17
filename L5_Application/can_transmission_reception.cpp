/*========================================
 ==========================================================================================
 Name        : can_transmission_reception.cpp
 Author      : Bhupendra Naphade
 Version     : 1.0
 Description : Implementation of CAN bus communication
 +=============================================================================================
 */
#include <can_transmission_reception.h>
#include "tasks.hpp"
#include "scheduler_task.hpp"
#include "sensor.hpp"
#include "can_communication_ids.h"
#include "sensor_defines.hpp"

//extern int front, back, right, left;
bool BootReplySent;         //Used as indication of system startup flag
bool reboot = 0;            // Used to indicate the reboot status
/* This function would initialize the CAN bus. It will setup the filter that is to be used for receiving
          messages. */
void can_Tx_Rx_init()
{
    CAN_init(can1, 100, 30, 30, NULL,NULL);
    can_std_grp_id_t *grplist=NULL;
    can_ext_id_t *ext=NULL;
    can_ext_grp_id_t *extgrp=NULL;
    if( CAN_setup_filter(canMessagesFilterList,4,grplist,0,ext,0,extgrp,0))  /*CAN message filter */
    CAN_reset_bus(can1); //enable the can bus
}
/* Previous can Tx function
void can_Tx_Sensor_data()
{
    can_msg_t msgTx;
   msgTx.msg_id = TSensorData;
   msgTx.frame_fields.is_29bit = 0;
   msgTx.frame_fields.data_len = 4;       // Send 8 bytes
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
}*/
//void can_Heart_beat()
//{
//    can_msg_t msgTx;
//   msgTx.msg_id = TBootReplyToMaster;
//   msgTx.frame_fields.is_29bit = 0;
//   msgTx.frame_fields.data_len = 1;       // Send bytes
//   msgTx.data.qword = 0x00;
//   if( !(CAN_tx(can1, &msgTx, 0)))
//   {
//      // LE.toggle(1) ;// printf("Heart beat not sent");
//   }
//}
//
//void can_Boot_stat()
//{
//    can_msg_t msgTx;
//   msgTx.msg_id = TBootReplyToMaster;
//   msgTx.frame_fields.is_29bit = 0;
//   msgTx.frame_fields.data_len = 1;       // Send bytes
//   msgTx.data.qword = 0x00;
//   if( !(CAN_tx(can1, &msgTx, 0)))
//   {
//       LE.toggle(1) ;// printf("Boot status not sent");
//   }
//}
//void CANTransmissionReception::
bool CANTransmit(uint32_t msg_id , uint8_t * data, uint32_t len)
{
    can_msg_t tx;
    uint8_t i;
    if(msg_id==THeartbeatToMaster)
    {
        LE.toggle(2);
    }
    tx.msg_id = msg_id;
    tx.frame_fields.is_29bit = 0;
    tx.frame_fields.data_len = len;
    for (i=0;i<len;i++)
    {
        tx.data.bytes[i] = data[i];
    }

    LE.toggle(4);
    //printf("Transmitted ID: %x\n",tx.msg_id);
    return(CAN_tx(can1,&tx,0));
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
                        if (BootReplySent == 0)
                        {
                            uint32_t bootData = BOOTREPLYDATA;
                            CANTransmit(TBootReplyToMaster,(uint8_t*)&bootData,sizeof(bootData));
                            BootReplySent = 1;
                            LE.on(1);
                        }
                        break;
                case RBootStatusFromMaster  :
                    //Future implementation
                        break;
            }
}
}
