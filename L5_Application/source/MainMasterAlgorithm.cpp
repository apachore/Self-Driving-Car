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


void SensorProcessingAlgorithm(can_msg_t canData)
{
    bool Straight, Left, Right, Rear;

    if(Straight)
    {
        //Send data to MotorFunction here in this project
    }
}

void GeoProcessingAlgorithm(can_msg_t canData)
{

}

MasterAlgorithm::MasterAlgorithm(uint8_t priority)
                : scheduler_task("MasterAlgorithm", 500, priority)
{
    motorStraight   = false;
    motorReverse    = false;
    motorLeft       = false;
    motorRight      = false;

    receivedDataQueue = scheduler_task::getSharedObject("receivedMessagesQueue");
}


bool MasterAlgorithm::run(void *p)
{
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
            }
            DecisionAlgorithm(canReceivedData);
        }
        else
        {
            LE.on(4);
        }
    }
    else
        LE.on(4);

    return true;
}

bool MasterAlgorithm::init(void)
{
    /*// HACK:
    data = *((gps_data_t*) scheduler_task::getSharedObject("gps_data"));*/


    return true;
}

void MasterAlgorithm::DecisionAlgorithm(can_msg_t canData)
{
    /* First check the sensor data from sensor message.
    Check sensor message in the messages enum */

    if(receivedSensorData.FrontDistance < 100)
    {
        if(receivedSensorData.LeftDistance < 100)
        {
            motorStraight   = true;
            motorLeft       = true;
        }
        if(receivedSensorData.RightDistance < 100)
        {
            motorStraight   = true;
            motorLeft       = true;
        }
        if(receivedSensorData.RightDistance < 100 && receivedSensorData.RightDistance < 100)
        {
            motorReverse = true;
        }
    }
}
