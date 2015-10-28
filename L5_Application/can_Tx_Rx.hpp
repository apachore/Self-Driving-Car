/*
 * can_Tx_Rx.hpp
 *
 *  Created on: Oct 27, 2015
 *      Author: Bhupe
 */

#ifndef L5_APPLICATION_CAN_TX_RX_HPP_
#define L5_APPLICATION_CAN_TX_RX_HPP_

#include "stdio.h"
#include "stdlib.h"
#include "can.h"

typedef enum
{
    HeartbeatToMaster     = 0x283, // Heart beat message to Master
    TBootStatToMaster     = 0x621, // Boot status to Master
    TDirectionSensorData  = 0x210, // Sensor data to IO and Master
    TBatteryStatus        = 0x420, // Battery status to IO and Master

    RBootStatEnquire      = 0x610, // Boot status enquire from Master
    RBootStatfromMaster   = 0x630,
    RKillMessage          = 0x020  // Kill Message
}CANMessageIds;

//
//class can_Tx_Rx
//{
//    public:
//                /* This function would initialize the CAN bus. It will setup the filter that is to be used for receiving
//           messages. */
//        void CANInit();
//        void CANTx(/*can_msg_t canMessageBlock*/);
//        void CANRx(/*can_msg_t canMessageBlock*/);
//
//
//    private:
//        QueueHandle_t canTxData_q;
//};











#endif /* L5_APPLICATION_CAN_TX_RX_HPP_ */
