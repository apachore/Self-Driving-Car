 /*
  * This file has all the Master Algorithm needed to drive the car.
  * To drive the car, this file has algorithm for detecting sensor data and taking
  * appropriate decision.
  *
  * Authored : Gaurao Chaudhari
  * Date     : 10/18/2015
  */

#include "stdio.h"
#include "string.h"
#include "MainMasterAlgorithm.h"
#include "io.hpp"
#include "can_communication_ids.h"
#include "can_transmission_reception.h"
#include "file_logger.h"
//#include "motorDrive.cpp"

#define straightSensorDangerDistance    50
#define turnSensorDangerDistance        30
#define lowestDangerDistance            20

#define fLow                    40
#define fLevel1                 80
#define fLevel2                 80

#define lLow                    40
#define lLevel1                 80
#define lLevel2                 80

#define rLow                    40
#define rLevel1                 80
#define rLevel2                 80


int countBrake = 0;

void MotorDriveFromSensors(bool frontMotor, bool reverseMotor, bool leftMotor, bool rightMotor, bool brakeFlag, uint8_t levelOfSpeed, uint8_t levelOfDirection)
{
    can_msg_t canMessage;

    canMessage.msg_id = TMotorControlToMotor;
    canMessage.frame_fields.is_29bit = 0;
    canMessage.frame_fields.data_len = 4;

    canMessage.data.bytes[0] = 0x0;
    canMessage.data.bytes[1] = 0x0;
    canMessage.data.bytes[2] = 0x0;
    canMessage.data.bytes[3] = 0x0;


    if(frontMotor)
    {
        canMessage.data.bytes[2] = 0x1;
        canMessage.data.bytes[3] = levelOfSpeed;
    }
    if(reverseMotor)
    {
        canMessage.data.bytes[2] = 0x2;
        canMessage.data.bytes[3] = levelOfSpeed;
    }
    if(leftMotor)
    {
        canMessage.data.bytes[0] = 0x1;
        canMessage.data.bytes[1] = levelOfDirection;
    }
    if(rightMotor)
    {
        canMessage.data.bytes[0] = 0x2;
        canMessage.data.bytes[1] = levelOfDirection;
    }

    CANTransmission(canMessage);
}

void SensorProcessingAlgorithm(can_msg_t canData, SensorData receivedSensorData)
{
    uint8_t levelOfSpeed = 0, levelOfDirection = 0;
    bool frontMotor = false;
    bool reverseMotor = false;
    bool leftMotor = false;
    bool rightMotor = false;
    bool brakeFlag = false;

    printf("%d\n", receivedSensorData.FrontDistance);
    printf("%d\n", receivedSensorData.LeftDistance);
    printf("%d\n\n", receivedSensorData.RightDistance);

    bool F1 = (receivedSensorData.FrontDistance < fLow) ? true : false;
    bool F2 = (receivedSensorData.FrontDistance < fLevel1 && receivedSensorData.FrontDistance > fLow) ? true : false;

    bool L1 = (receivedSensorData.LeftDistance < lLow) ? true : false;
    bool L2 = (receivedSensorData.LeftDistance < lLevel1 && receivedSensorData.LeftDistance > lLow) ? true : false;

    bool Rt1 = (receivedSensorData.RightDistance < rLow) ? true : false;
    bool Rt2 = (receivedSensorData.RightDistance < rLevel1 && receivedSensorData.RightDistance > rLow) ? true : false;

    bool Reverse = (receivedSensorData.RearDistance < 60) ? true : false;

    /*if(!F1 && !F2 && !L1 && !L2 && !Rt1 && !Rt2) {
        frontMotor = true;
        levelOfSpeed = 1;
    }
    else */if (F1) {
        //LE.toggle(1);
        if (!Reverse) {
            LE.toggle(4);
            reverseMotor = true;
            levelOfSpeed = 2;
            if (receivedSensorData.LeftDistance > receivedSensorData.RightDistance) {
                rightMotor = true;
                levelOfDirection = 5;
            }
            else if (receivedSensorData.RightDistance > receivedSensorData.LeftDistance) {
                leftMotor = true;
                levelOfDirection = 5;
            }
        }
    }
    else if (F2) {
        LE.toggle(1);
        frontMotor = true;
        levelOfSpeed = 1;
        if (L1 && !Rt1 && !Rt2) {
            // Extreme Right;
            LE.toggle(2);
            rightMotor = true;
            levelOfDirection = 5;
        }
        else if (L2 && !Rt1 && !Rt2) {
            LE.toggle(2);
            rightMotor = true;
            levelOfDirection = 3;
            // Less level of right
        }
        else if (Rt1 && !L1 && !L2) {
            LE.toggle(3);
            leftMotor = true;
            levelOfDirection = 5;
            // Extremen Left;
        }
        else if (Rt2 && !L1 && !L2) {
            LE.toggle(3);
            leftMotor = true;
            levelOfDirection = 3;
            // Less level of left;
        }
    }
    else if (L1 || L2) {
        printf("Left");
        LE.toggle(2);
        frontMotor = true;
        levelOfSpeed = 1;
        if (L1 && !Rt1 && !Rt2) {
            // Extreme right;
            rightMotor = true;
            levelOfDirection = 5;
        }
        else if (L2 && !Rt1 && !Rt2) {
            // Less level of right;
            rightMotor = true;
            levelOfDirection = 3;
        }
    }
    else if (Rt1 || Rt2) {
        printf("Right");
        LE.toggle(3);
        frontMotor = true;
        levelOfSpeed = 1;
        if (Rt1 && !L1 && !L2) {
            // Extreme left;
            leftMotor = true;
            levelOfDirection = 5;

        }
        else if (Rt2 && !L1 && !L2) {
            // Less level of left;
            leftMotor = true;
            levelOfDirection = 3;
        }
    }

    MotorDriveFromSensors(frontMotor, reverseMotor, leftMotor, rightMotor, brakeFlag, levelOfSpeed, levelOfDirection);
}

void GeoProcessingAlgorithm(can_msg_t canData)
{

}

void DecisionAlgorithm(can_msg_t canReceivedData)
{
    SensorData receivedSensorData;

    if(canReceivedData.msg_id == RSensorDataFromSensor)
    {
        receivedSensorData.FrontDistance    = canReceivedData.data.bytes[0];
        receivedSensorData.LeftDistance     = canReceivedData.data.bytes[1];
        receivedSensorData.RightDistance    = canReceivedData.data.bytes[2];
        receivedSensorData.RearDistance     = canReceivedData.data.bytes[3];
        SensorProcessingAlgorithm(canReceivedData, receivedSensorData);
    }
    else
    {
        MotorDriveFromSensors(true, false, false, false, false, 1, 1);
    }
}
