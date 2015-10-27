/*
 * can.hpp
 *
 *  Created on: 27-Oct-2015
 *      Author: User
 */

#ifndef L5_APPLICATION_CAN_COMMS_HPP_
#define L5_APPLICATION_CAN_COMMS_HPP_

#include "stdio.h"
/*
 *
 * Message ids that shall be accepted and transmitted by IO module
 */
typedef enum
{
    RkillmsgAndroid =0x610,
    Rdatafromsensor =0x210,
    Rdatafromgps =0x260,
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
         CAN_gen_sid(can1, Rdatafromgps),
         CAN_gen_sid(can1, Rdestreachedfrommaster),
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

         uint32_t Final_Dist;
         uint32_t nextCheckpoint_Dist;

         uint16_t Heading_Angle;           // Heading angle
         uint16_t Bearing_Angle;           // Bearing angle
 } Geo_Data;


#endif /* L5_APPLICATION_CAN_COMMS_HPP_ */
