/*
The function motorDrive in this file constructs the messages that needs to be sent to the
motor to drive itself. The messages are tailored according to the needs of Decision Algorithm
in the file 'MainMaster Algorithm'.
As per the outcome of Sensor and Geo based Algorithm this section of code will send CAN messages to the Motor Module to drive the Motor.
Author: Akshay Dabholkar
Date Created : 18/10/2015
*/
/*
Currently in nascent stage, will take up shape once starts receiving data bytes from  Geo and Sensor Modules, currently can use pseudo-code or stubs for its
implementation.
*/


#include "MainMasterAlgorithm.h"
#include "can_communication_ids.h"
#include "tasks.hpp"
#include "io.hpp"
#include "utilities.h"
#include "can_transmission_reception.h"
#include "can.h"
#include "stdio.h"


/*
 * The use of this function would be decided later. For now keeping this function and not removing.
 */
void MotorDriveFromSensor(bool motorStraight, bool motorReverse, bool motorLeft, bool motorRight, uint8_t levelofspeed, uint8_t levelofdirection)
{
    can_msg_t canMessage;
    canMessage.msg_id = TMotorControlToMotor;
    canMessage.frame_fields.is_29bit = 0;
    canMessage.frame_fields.data_len = 4;

    if(motorStraight)
    {
        canMessage.data.bytes[0]=0x0;
        canMessage.data.bytes[1]=0x0;
        canMessage.data.bytes[2]=0x1;
        canMessage.data.bytes[3]=levelofspeed;
    }

    if(motorReverse)
    {
        canMessage.data.bytes[0]=0x0;
        canMessage.data.bytes[1]=0x0;
        canMessage.data.bytes[2]=0x2;
        canMessage.data.bytes[3]=levelofspeed;
    }

    if(motorLeft)
    {
        canMessage.data.bytes[0]=0x1;
        canMessage.data.bytes[1]=levelofdirection;
    }

    if(motorRight)
    {
        canMessage.data.bytes[0]=0x2;
        canMessage.data.bytes[1]=levelofdirection;
    }

    canMessage.msg_id = TMotorControlToMotor;
    //CANTransmission(canMessage.msg_id, &canMessage.data.bytes[0], 4);
}

