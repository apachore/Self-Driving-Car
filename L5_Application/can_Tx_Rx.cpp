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


extern int front, back, right, left;
void can_Tx_Rx_init()
{
    CAN_init(can1, 100, 100, 100, NULL,NULL);
            CAN_bypass_filter_accept_all_msgs();//Receive all can message
            CAN_reset_bus(can1); //enable the can bus
}

void can_Tx_Sensor_data()
{
    can_msg_t msgTx;
   msgTx.msg_id = TDirectionSensorData;
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
        LE.toggle(1) ; //printf("Sensor data not sent");
   }
   else
   {
       LE.toggle(4);
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
       LE.toggle(1) ;// printf("Heart beat not sent");
   }
}

void can_Boot_stat()
{
    can_msg_t msgTx;
   msgTx.msg_id = TBootStatToMaster;
   msgTx.frame_fields.is_29bit = 0;
   msgTx.frame_fields.data_len = 1;       // Send bytes
   msgTx.data.qword = 0x00;
   if( !(CAN_tx(can1, &msgTx, 0)))
   {
       LE.toggle(1) ;// printf("Boot status not sent");
   }
}

//can_Tx_Rx::can_Tx_Rx(uint8_t priority) : scheduler_task("canTxTask", 4*512, priority),
//canTxData_q(NULL)
//        {
//        }
//bool can_Tx_Rx::CANInit()
//{
//
//   }

