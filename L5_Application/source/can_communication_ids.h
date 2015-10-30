/*
 * can_communication_ids.h
 *
 * Enumeration for the CAN message ID's that Master Module is expected to receive and
 * transmit.
 *
 *  Created on: Oct 24, 2015
 *  Author: Akshay Dabholkar and Gaurao Chaudhari
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
    // Boot Request to all Controllers
    TBootRequestToAll                = 0x610,

    // Boot Replies from all Controllers
    RBootReplyFromMotor              = 0x620,
    RBootReplyFromSensor             = 0x621,
    RBootReplyFromGeo                = 0x622,
    RBootReplyFromAndroid            = 0x623,

    // Boot Status to all Controllers
    // This message will enable the map and run button in Android to be handled at Android end
    TBootStatusToAll                 = 0x630,            // To all

    // Messages between Master and Motor
    // To Motor
    TMotorControlToMotor             = 0x220,
    TStopToMotor                     = 0x040,          // Stop message from Master to Motor.

    // Messages between Master and Sensor
    // From Sensor
    RSensorDataFromSensor            = 0x210,
    RBatteryStatusFromSensor         = 0x420,

    // Messages between Master and Geo
    // From Geo
    RDistanceFinalAndNextCheckpoint  = 0x250,          // distance to go.
    RHeadingAndBearingToGeo          = 0x260,

    // Messages between Master and Android
    // From Android
    RStopMessageFromAndroid          = 0x030,
    RKillMessageFromAndroid          = 0x010,          // Kill from Android to Master.
    //Sys_start,
    RRunAndPauseCommandFromAndroid   = 0x270,          // To make the car run and pause

    // Heartbeat message from all controllers
    RHeartbeatMessageFomMotor        = 0x280,
    RHeartbeatFromGeo                = 0x281,
    RHeartbeatFromAndroid            = 0x282,
    RHeartbeatFromSensor             = 0x283,

    // Other Messages
    TDestinationReachedToAll         = 0x410,
    TKillMessageToAll                = 0x020,
} CANMessageIds;


/* Following are the HW filter setup for the messages to be received by Master from all
 * Modules. Message ID's can be subjected to change as the project proceeds, as few new
 * messages can be added or removed.
 */
const can_std_id_t canMessagesFilterList[] =
{
        CAN_gen_sid(can1, RKillMessageFromAndroid),CAN_gen_sid(can1, RStopMessageFromAndroid),  CAN_gen_sid(can1, RSensorDataFromSensor),
        CAN_gen_sid(can1, RDistanceFinalAndNextCheckpoint), CAN_gen_sid(can1, RHeadingAndBearingToGeo),
        CAN_gen_sid(can1, RRunAndPauseCommandFromAndroid), CAN_gen_sid(can1, RBatteryStatusFromSensor)
};

const can_std_grp_id_t groupList[] =
{
        {CAN_gen_sid(can1, RHeartbeatMessageFomMotor), CAN_gen_sid(can1, RHeartbeatFromSensor)},
        {CAN_gen_sid(can1, RBootReplyFromMotor), CAN_gen_sid(can1, RBootReplyFromAndroid)}
};


// Struct for Motor Data
typedef struct
{
        // Message 0x220 (4 bytes)
        uint8_t leftRightdirection;
        uint8_t levelOfDirection;
        uint8_t frontBackDirection;
        uint8_t levelOfSpeed;
} MotorData;

// Struct for Sensor Data
typedef struct
{
        // Message 0x210 (4 bytes)
        uint8_t FrontDistance;          // Forward distance in cms
        uint8_t RearDistance;           // Rear distance in cms
        uint8_t LeftDistance;           // Left distance in cms
        uint8_t RightDistance;          // Right distance in cms
} SensorData;

// Struct for Geo Data
typedef struct
{
        //Message 0x250 (8 bytes)
        uint32_t finalDistance;          // Final distance to the destination
        uint32_t nextCheckpointDistance; // Next checkpoint distance to the destination

        //Message 0x260 (4 bytes)
        uint16_t HeadingAngle;           // Heading angle with respect to North
        uint16_t BearingAngle;           // Bearing angle with respect to North
} GeoData;


#endif /* L5_APPLICATION_SOURCE_CAN_COMMUNICATION_IDS_H_ */
