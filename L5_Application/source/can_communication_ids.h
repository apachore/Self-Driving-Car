/*
 * can_communication_ids.h
 *
 * Enumeration for the CAN message ID's that Master Module is expected to receive and
 * transmit.
 *
 *  Created on: Oct 24, 2015
 *  Author: Original code:Akshay Dabholkar and Gaurao Chaudhari
 *          Modified by:Amit Pachore
 *          Modified on:Oct 26, 2015
 *          Modified for GeoController specific CAN messages
 */

#ifndef L5_APPLICATION_SOURCE_CAN_COMMUNICATION_IDS_H_
#define L5_APPLICATION_SOURCE_CAN_COMMUNICATION_IDS_H_


 /* Message ID's are subjected to change as projects proceeds as more number of messages
  * can be added or deleted. Messages with blank ID's are still under discussion
  */

#include "can.h"
#include "stdio.h"


typedef enum
{
    TStartToMaster                    =0x270,
    TStopToMaster                     =0x030,
    TKillToMaster                     =0x010,
    TDestToGeo                        =0x240,
    RSourceFromGeo                    =0x650,
/*    // Boot messages
    RBootRequestFromMaster            = 0x610,
    TBootReplyToMaster                = 0x622,

    // This message will enable the map and run button in Android to be handled at Android end

    //In Geo Module this message can be looked to transmit coordinates to Android
    //As soon as this message is received transmit coordinates(Initial Logic on 25 Oct 15)
    RBootStatusFromMaster             = 0x630,
    // Messages between Master and Geo
    // From Geo
    TFinalAndNextCheckpointDistance   = 0x250,
    THeadingAndBearingToMaster        = 0x260,

    // Messages between Geo and Android
    // From Android
    TSourceCoordinates                = 0x650,
    RCheckpointsFromAndroid           = 0x240,
    RTotalTravelDistanceFromAndroid   = 0x230,

    // Heart beat message to Master
    THeartbeatToMaster                = 0x281,

    // Other Messages
    RKillMessageFromMaster            = 0x020,*/

    //DIsabled Filter ID - Used to make Standard ID count even
    FilterDisabledCANmsgID               = 0xFFFF

} CANMessageIds;


/* Following are the HW filter setup for the messages to be received by Geo from other
 * Modules. Message ID's can be subjected to change as the project proceeds, as few new
 * messages can be added or removed.
 */
const can_std_id_t canMessagesFilterList[] =
{
       /* CAN_gen_sid(can1, RKillMessageFromMaster),         MSG ID = 0x020
        CAN_gen_sid(can1, RTotalTravelDistanceFromAndroid),MSG ID = 0x230
        CAN_gen_sid(can1, RCheckpointsFromAndroid),        MSG ID = 0x240
        CAN_gen_sid(can1, RBootRequestFromMaster),         MSG ID = 0x610
        CAN_gen_sid(can1, RBootStatusFromMaster),          MSG ID = 0x630*/
        CAN_gen_sid(can1,RSourceFromGeo),                  //MSG ID = 0x650
        CAN_gen_sid(can1, FilterDisabledCANmsgID)          /*MSG ID = 0xFFFF*/
};


#endif /* L5_APPLICATION_SOURCE_CAN_COMMUNICATION_IDS_H_ */
