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


#include "MainMasterAlgorithm.cpp"
#include "can_communication_ids.h"
#include "scheduler_task.hpp"
#include "tasks.hpp"
#include "io.hpp"
#include "utilities.h"
#include "can.h"
#include "stdio.h"


 can_msg_t canmsg;

void MotorDriveFromSensor(bool motorStraight, bool motorReverse, bool motorLeft, bool motorRight,uint8_t levelofspeed,uint8_t levelofdirection)
{
    canmsg.msg_id=TMotorControlToMotor;
     canmsg.frame_fields.is_29bit=0;
     canmsg.frame_fields.data_len=4;

    if(motorStraight)
      {
        canmsg.data.bytes[0]=0x0;
        canmsg.data.bytes[1]=0x0;
        canmsg.data.bytes[2]=0x1;
        canmsg.data.bytes[3]=levelofspeed;

      }
    if(motorReverse)
          {
           canmsg.data.bytes[0]=0x0;
           canmsg.data.bytes[1]=0x0;
           canmsg.data.bytes[2]=0x2;
           canmsg.data.bytes[3]=levelofspeed;

          }
    if(motorLeft)
       {
        canmsg.data.bytes[0]=0x1;
        canmsg.data.bytes[1]=levelofdirection;

       }

    if(motorRight)
      {
       canmsg.data.bytes[0]=0x2;
       canmsg.data.bytes[1]=levelofdirection;

      }


    CANTransmissionReception::CANTransmission(canmsg);


}

