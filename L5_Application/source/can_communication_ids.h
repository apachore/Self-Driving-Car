#ifndef L5_APPLICATION_SOURCE_CAN_COMMUNICATION_IDS_H_
#define L5_APPLICATION_SOURCE_CAN_COMMUNICATION_IDS_H_


 /* Message ID's are subjected to change as projects proceeds as more number of messages
  * can be added or deleted.
  */

#include "can.h"
#include "stdio.h"

typedef enum
{

    /* CAN Communication with Master Module*/
    TStartToMaster                    = 0x270,
    TStopToMaster                     = 0x030,
    TKillToMaster                     = 0x010,
    THeartbeatToMaster                = 0x282,
    TBootReplyToMaster                = 0x623,
    RDestReachedFromMaster            = 0x410,
    RBootReqFromMaster                = 0x610,
    RBootStatReqFromMaster            = 0x630,
    RKillFromMaster                   = 0x020,

    /* CAN Communication with Geo Module*/
    TDestToGeo                        = 0x240,
    TDistanceToGeo                    = 0x230,
    RSourceFromGeo                    = 0x650,
    TResetToGeo                       = 0x241,

    /* CAN Communication with Sensor Module*/
    RSensorDataFromSensors            = 0x210,

    /* CAN Communication with Motor Module*/
    RBatteryStatFromMotor             = 0x420,

    //DIsabled Filter ID - Used to make Standard ID count even
    FilterDisabledCANmsgID            = 0xFFFF

} CANMessageIds;


/* Following are the HW filter setup for the messages to be received by Android from other
 * Modules. Message ID's can be subjected to change as the project proceeds, as few new
 * messages can be added or removed.
 */
const can_std_id_t canMessagesFilterList[] =
{
        CAN_gen_sid(can1,RKillFromMaster),               //MSG ID = 0x020
        CAN_gen_sid(can1,RSensorDataFromSensors),        //MSG ID = 0x210
        CAN_gen_sid(can1,RDestReachedFromMaster),        //MSG ID = 0x410
        CAN_gen_sid(can1,RBatteryStatFromMotor),         //MSG ID = 0x420
        CAN_gen_sid(can1,RBootReqFromMaster),            //MSG ID = 0x610
        CAN_gen_sid(can1,RBootStatReqFromMaster),        //MSG ID = 0x630
        CAN_gen_sid(can1,RSourceFromGeo),                //MSG ID = 0x650
        CAN_gen_sid(can1,FilterDisabledCANmsgID)         //MSG ID = 0xFFFF
};


#endif /* L5_APPLICATION_SOURCE_CAN_COMMUNICATION_IDS_H_ */
