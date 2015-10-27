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
    // Boot messages
    RBootRequestFromMaster            = 0x610,
    TBootReplyToMaster                = 0x622,

    // This message will enable the map and run button in Android to be handled at Android end

    //In Geo Module this message can be looked to transmit coordinates to Android
    //As soon as this message is received transmit coordinates(Initial Logic on 25 Oct 15)
    RBootStatusFromMaster             = 0x630,            // To all

    // Messages between Master and Geo
    // From Geo
    TFinalDistanceAndNextCheckpoint   = 0x250,          // distance to go.
    THeadingAndBearingToMaster        = 0x260,

    // Messages between Geo and Android
    // From Android
    TSourceCoordinatesToAndroid       = 0x650,
    RCheckpointsFromAndroid           = 0x240,
    RTotalTravelDistanceFromAndroid   = 0x230,

    // Heart beat message to Master
    THeartbeatToMaster                = 0x281,

    // Other Messages
    RKillMessageFromMaster            = 0x020,
} CANMessageIds;


/* Following are the HW filter setup for the messages to be received by Geo from other
 * Modules. Message ID's can be subjected to change as the project proceeds, as few new
 * messages can be added or removed.
 */
const can_std_id_t canMessagesFilterList[] =
{
        CAN_gen_sid(can1, RBootRequestFromMaster) , CAN_gen_sid(can1,RBootStatusFromMaster),
        CAN_gen_sid(can1, RCheckpointsFromAndroid), CAN_gen_sid(can1, RTotalTravelDistanceFromAndroid),
        CAN_gen_sid(can1, RKillMessageFromMaster)
};


#endif /* L5_APPLICATION_SOURCE_CAN_COMMUNICATION_IDS_H_ */
