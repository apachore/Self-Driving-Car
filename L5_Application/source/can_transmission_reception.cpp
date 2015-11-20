/*
 * can_transmission_reception.cpp
 *
 *  Created on: Oct 24, 2015
 *      Author: Gaurao Chaudhari
 */


#include "stdio.h"
#include "stdlib.h"
#include "can.h"
#include "io.hpp"
#include "can_communication_ids.h"
#include "can_transmission_reception.h"
#include "scheduler_task.hpp"
#include "MainMasterAlgorithm.h"
#include "file_logger.h"
#include "string.h"

// XXX: The callback is called from inside CAN Bus interrupt, should not use printf() here
// XXX: CAN Bus reset should not be called right away, it should reset maybe in 10Hz if can bus is off

void DataOverCanBuffer(uint32_t param)
{
    LOG_ERROR("Data over CAN buffer");
    /*CAN_reset_bus(can1);*/
}

void CANInitialization()
{
    CAN_init(can1, 100, 500, 500, NULL, *DataOverCanBuffer);
    CAN_reset_bus(can1);

    // Create a queue which would send the data to wherever received
    /*receivedDataQueue = xQueueCreate(10, sizeof(can_msg_t));
    scheduler_task::addSharedObject("receivedMessagesQueue", receivedDataQueue);*/

    //Any message with 0xffff would disable the message
    //CAN_bypass_filter_accept_all_msgs();
    CAN_setup_filter(canMessagesFilterList, 8, groupList, 2, NULL, 0, NULL, 0);
}


void CANTransmission(uint32_t msg_id, uint64_t* data, uint32_t length)
{
    can_msg_t canMessageBlock = {0};
    uint8_t i;
    canMessageBlock.msg_id = msg_id;
    canMessageBlock.frame_fields.is_29bit = 0;
    canMessageBlock.frame_fields.data_len = length;
    canMessageBlock.data.bytes[0] = data;
    canMessageBlock.data.qword = data;

    CAN_tx(can1,&canMessageBlock, 0);

    //LOG_ERROR("Message %d not transmitted", canMessageBlock.msg_id);
}

bool CANReception(can_msg_t& canMessageBlock)
{
    // XXX: Cannot block from the periodic function
    if(CAN_rx(can1, &canMessageBlock, 0))
    {
        switch (canMessageBlock.msg_id)
        {
            case RSensorDataFromSensor:
                SensorData receivedSensorData;
/*                receivedSensorData.FrontDistance    = canMessageBlock.data.words[0];
                receivedSensorData.LeftDistance     = canMessageBlock.data.words[1];
                receivedSensorData.RightDistance    = canMessageBlock.data.words[2];
                receivedSensorData.RearDistance     = canMessageBlock.data.words[3];*/
                receivedSensorData.FrontDistance    = canMessageBlock.data.bytes[0];
                receivedSensorData.LeftDistance     = canMessageBlock.data.bytes[1];
                receivedSensorData.RightDistance    = canMessageBlock.data.bytes[2];
                receivedSensorData.RearDistance     = canMessageBlock.data.bytes[3];
                memcpy(&receivedSensorData, &canMessageBlock.data, sizeof(receivedSensorData));
                SensorProcessingAlgorithm(receivedSensorData);
                break;

            case RKillMessageFromAndroid:
                SendKillMessageToAllControllers();
                break;

            case RBootReplyFromAndroid:

                break;

            case RBootReplyFromGeo:
                break;

            case RBootReplyFromMotor:
                break;

            case RBootReplyFromSensor:
                break;

            default:
                MotorDriveFromSensors(true, false, false, false, false, SpeedLevel3, 0);
                break;
        }
        return true;
    }
    else
    {
        /*LOG_ERROR("CAN not receiving messages");*/
        return false;
    }

}
