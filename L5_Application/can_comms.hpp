/*
 * can.hpp
 *
 *  Created on: 27-Oct-2015
 *      Author: User
 */

#ifndef L5_APPLICATION_CAN_COMMS_HPP_
#define L5_APPLICATION_CAN_COMMS_HPP_

#include "stdio.h"
#include "can.h"
/*
 *
 * Message ids that shall be accepted and transmitted by IO module
 */
typedef enum
{
    RkillmsgAndroid =0x010,
    Rdatafromsensor =0x210,
    Rlatlongfromgps =0x240,
    Rdistremainfromgps=0x250,
    Rdestreachedfrommaster =0x410,
    Rbootreqfrommaster =0x610,
    RBootstatusfrommaster= 0x630,
    Tbootreplytomaster =0x620

 }canmsgIds;

 /*
  * configuring HW filter to receive messages from many modules
  *
  */

 const can_std_id_t CANmsglist[] =
 {
         CAN_gen_sid(can1, RkillmsgAndroid),
         CAN_gen_sid(can1, Rdatafromsensor),
         CAN_gen_sid(can1, Rlatlongfromgps),
         CAN_gen_sid(can1, Rlatlongfromgps),
         CAN_gen_sid(can1, Rdistremainfromgps),
         CAN_gen_sid(can1, Rbootreqfrommaster),
         CAN_gen_sid(can1, RBootstatusfrommaster)

 };
 /*
  * sensor data
  *
  */
 typedef struct
 {
         uint8_t Front_Dist;
         uint8_t Rear_Dist;
         uint8_t Left_Dist;
         uint8_t Right_Dist;

 }Sensor_data;
/*
 *
 *
 */

 typedef struct
 {
         uint16_t Dist_Remaining;
         float Latitude;
         float Longitude;
 } Geo_Data;
typedef struct
{
   can_msg_t canReceivedData;
   can_msg_t canReceivedMsg_gps1;
   can_msg_t canReceivedMsg_gps2;
   can_msg_t canReceivedMsg_Sensor;
}Can_msg;

#endif /* L5_APPLICATION_CAN_COMMS_HPP_ */
