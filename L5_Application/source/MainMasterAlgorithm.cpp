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

void MotorDriveFromSensors(bool frontMotor, bool reverseMotor, bool leftMotor, bool rightMotor, uint8_t levelOfSpeed, uint8_t levelOfDirection)
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
    bool frontMotor = false;
    bool reverseMotor = false;
    bool leftMotor = false;
    bool rightMotor = false;

    /* First check the sensor data from sensor message.
    Check sensor message in the messages enum */
    if(receivedSensorData.FrontDistance < 30 && receivedSensorData.LeftDistance < 30)
    {
        frontMotor = true;
        rightMotor = true;
    }
    else if(receivedSensorData.FrontDistance < 30 && receivedSensorData.RightDistance < 30)
    {
        frontMotor = true;
        leftMotor = true;
    }
    else if(receivedSensorData.FrontDistance < 30 && receivedSensorData.RightDistance < 30
            && receivedSensorData.RightDistance)
    {
        reverseMotor = true;
        leftMotor = true;           //Can make either left or right movement. Need to check afterwards.
    }
/*    if(receivedSensorData.FrontDistance > 30 && receivedSensorData.LeftDistance > 30
            && receivedSensorData.RightDistance > 30 && receivedSensorData.FrontDistance > 30)
    {
        frontMotor = true;
        reverseMotor = true;
        leftMotor = true;
        rightMotor = true;
    }*/

    //CANTransmission(canMessage);
    MotorDriveFromSensors(frontMotor, reverseMotor, leftMotor, rightMotor, 1, 1);
}

void GeoProcessingAlgorithm(can_msg_t canData)
{

}

void DecisionAlgorithm(can_msg_t canData)
{
    static QueueHandle_t receivedDataQueue = scheduler_task::getSharedObject("receivedMessagesQueue");
    can_msg_t canReceivedData;
    SensorData receivedSensorData;

    if (xQueueReceive(receivedDataQueue, &canReceivedData, 0))
    {
        if (receivedDataQueue != NULL)
        {
            if(canReceivedData.msg_id == CANMessageIds::RSensorDataFromSensor)
            {
                receivedSensorData.FrontDistance    = canReceivedData.data.bytes[0];
                receivedSensorData.LeftDistance     = canReceivedData.data.bytes[1];
                receivedSensorData.RightDistance    = canReceivedData.data.bytes[2];
                receivedSensorData.RearDistance     = canReceivedData.data.bytes[3];
                SensorProcessingAlgorithm(canReceivedData, receivedSensorData);
            }
        }
        else
        {
            LOG_ERROR("CAN not receiving messages");
        }
    }
    else
        LE.toggle(3);
}
