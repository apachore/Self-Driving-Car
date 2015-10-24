/*
 * Created Enumeration for the CAN message ID's that Master Module is expected to receive and
 * transmit. Also created a function in which filter is setup for the messages to be received.
   Author: Akshay Dabholkar
   Created: 10/22/2015
 */

 /* Message ID's are subjected to change as projects proceeds as more number of messages can be added or deleted.
 Messages with blank ID's are still under discussion*/

#include "can.h"
#include "stdio.h"
enum can_msgs
{
    /* From Master to Android : */
    Boot_status=0x630,                    // to Android and IO

    /* From Master to Motor/IO */
    MastertoMotorControl=0x220,
    Stop_Ms_to_M=0x030,                  // Stop message from Master to Motor.

   /* From Android to Master */
    Kill_A_to_Ms=0x010,                  // Kill from Android to Master.
    Stop_A_to_Ms=0x020,                  // Stop message from Android to Master.
    Sys_start,
    Run_A_to_Ms=0x270,

    /* From Geo to Master */
    D_to_G=0x250,                         // distance to go.
    Heading_bearing=0x260,


    /* Sensor to Master  */
    Sensor_val=0x210,
    battery_status=0x420,

    /* From Master to all */
    Boot_request=0x610,
    Destination_reached,
    Kill_Ms_to_all,

    /* From all to Master */
    Heart_beat_M=0x280,
    Heart_beat_G=0x281,
    Heart_beat_A=0x282,
    Heart_beat_S=0x283,

    Boot_reply_M=0x620,
    Boot_reply_S=0x621,
    Boot_reply_G=0x622,
    Boot_reply_A=0x623


};

/* Following are the HW filter setup for the messages to be received by Master from all
 * Modules. Message ID's can be subjected to change as the project proceeds, as few new
 * messages can be added or removed.
 */

void set_up_filter(void)
{
    const can_std_id_t message_list[] = { CAN_gen_sid(can1,Kill_A_to_Ms) , CAN_gen_sid(can1,Stop_A_to_Ms),
                                          CAN_gen_sid(can1,Sensor_val), CAN_gen_sid(can1,D_to_G),
                                          CAN_gen_sid(can1,Heading_bearing),CAN_gen_sid(can1,Run_A_to_Ms),
                                          CAN_gen_sid(can1,battery_status)
                                        };
/*  Following are two Groups created for Heart_beat messages and Boot_reply messages from
 *  all the controllers.
 */
    const can_std_grp_id_t group_list[] = { {CAN_gen_sid(can1, Heart_beat_M), CAN_gen_sid(can1,Heart_beat_S)},
                                            {CAN_gen_sid(can1,Boot_reply_M), CAN_gen_sid(can1,Boot_reply_A)}
                                          };

    const can_ext_id_t *ext_list =NULL;
    const can_ext_grp_id_t *ext_group =NULL;
   CAN_setup_filter(message_list,7,group_list,2,ext_list,0,ext_group,0);
}


