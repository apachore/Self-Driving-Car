/*
 * can_communication_ids.h
 *
 * Enumeration for the CAN message ID's that Master Module is expected to receive and
 * transmit.
 *
 *  Created on: Oct 24, 2015
 *  Author: Original code:Akshay Dabholkar and Gaurao Chaudhari
 *          Modified by:Bhupendra Naphade
 *          Modified on:Dec 8, 2015
 *          Modified for Sensor Controller specific CAN messages
 */


#ifndef L5_APPLICATION_SOURCE_CAN_COMMUNICATION_IDS_H_
#define L5_APPLICATION_SOURCE_CAN_COMMUNICATION_IDS_H_

#include "can.h"

typedef enum
{
    THeartbeatToMaster     = 0x283, // Heart beat message to Master

    RBootRequestFromMaster      = 0x610, // Boot request from Master
    TBootReplyToMaster          = 0x621, // Boot status to Master

    TSensorData  = 0x210, // Sensor data to IO and Master
    TBatteryStatus        = 0x420, // Battery status to IO and Master


    RBootStatusFromMaster   = 0x630,
    RKillMessage          = 0x020,  // Kill Message
    RSensorModuleStart    = 0x640,  //Start message from Android

    //DIsabled Filter ID - Used to make Standard ID count even
     FilterDisabledCANmsgID               = 0xFFFF
}CANMessageIds;

/* Following are the HW filter setup for the messages to be received by Geo from other
 * Modules. Message ID's can be subjected to change as the project proceeds, as few new
 * messages can be added or removed.
 */
const can_std_id_t canMessagesFilterList[] =
{
        CAN_gen_sid(can1, RKillMessage),                   /*MSG ID = 0x020*/
        CAN_gen_sid(can1, RBootRequestFromMaster),         /*MSG ID = 0x610*/
        CAN_gen_sid(can1, RBootStatusFromMaster),          /*MSG ID = 0x630*/
        CAN_gen_sid(can1, FilterDisabledCANmsgID)          /*MSG ID = 0xFFFF*/

};

#endif /* L5_APPLICATION_SOURCE_CAN_COMMUNICATION_IDS_H_ */
