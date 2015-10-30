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
#include "queue.h"
#include "file_logger.h"

QueueHandle_t receivedDataQueue;

// XXX: The callback is called from inside CAN Bus interrupt, should not use printf() here
// XXX: CAN Bus reset should not be called right away, it should reset maybe in 10Hz if can bus is off

void BusOffCb(uint32_t param)
{
    LOG_ERROR("CAN Bus off");
    /*CAN_reset_bus(can1);*/
}

void DataOverCanBuffer(uint32_t param)
{
    LOG_ERROR("Data over CAN buffer");
    /*CAN_reset_bus(can1);*/
}

void CANInitialization()
{
    CAN_init(can1, 500, 500, 500, *BusOffCb, *DataOverCanBuffer);
    CAN_reset_bus(can1);

    // Create a queue which would send the data to wherever received
    receivedDataQueue = xQueueCreate(10, sizeof(can_msg_t));
    scheduler_task::addSharedObject("receivedMessagesQueue", receivedDataQueue);

    //Any message with 0xffff would disable the message
    CAN_bypass_filter_accept_all_msgs();
    //CAN_setup_filter(canMessagesFilterList, 7, groupList, 2, NULL, 0, NULL, 0);
}

void CANTransmission(can_msg_t canMessageBlock)
{
    if(CAN_tx(can1, &canMessageBlock, 0))
    {
        LE.toggle(3);
    }
    else
    {
        //LOG_ERROR("Message %d not transmitted", canMessageBlock.msg_id);
        LE.toggle(4);
    }
}

void CANReception(can_msg_t canMessageBlock)
{
    // XXX: Cannot block from the periodic function
    if(CAN_rx(can1, &canMessageBlock, 0))
    {
        LE.toggle(1);
        if (!xQueueSend(receivedDataQueue, &canMessageBlock, 0))
        {
            //LOG_ERROR("receivedDataQueue: Queue is not sending data.");
        }
    }
    else
    {
        LE.toggle(2);
        /*LOG_ERROR*/("CAN not receiving messages");
    }

}
