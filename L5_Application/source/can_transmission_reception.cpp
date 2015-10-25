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


CANTransmissionReception::CANTransmissionReception()
{
    if(receivedDataQueue == NULL)
        receivedDataQueue = xQueueCreate(50, sizeof(can_msg_t));
}

void BusOffCb(uint32_t param)
{
    printf("BusOffCb : Error");
    CAN_reset_bus(can1);
}

void DataOverCanBuffer(uint32_t param)
{
    printf("DataOverCanBuffer : Error");
    CAN_reset_bus(can1);
}

void CANTransmissionReception::CANInitialization()
{
    CAN_init(can1, 100, 20, 20, *BusOffCb, *DataOverCanBuffer);

    // Create a queue which would send the data to wherever received
    QueueHandle_t receivedDataQueue = xQueueCreate(50, sizeof(can_msg_t));
    scheduler_task::addSharedObject("receivedMessagesQueue", receivedDataQueue);

    /* HACK:
        addSharedObject("gps_data", &gps_data);*/

    //Any message with 0xffff would disable the message
    CAN_setup_filter(canMessagesFilterList , 15, NULL, 0, NULL, 0, NULL, 0);
}

void CANTransmissionReception::CANTransmission(/*can_msg_t canMessageBlock*/)
{
/*    canMessageBlock.msg_id = 0x2;
    canMessageBlock.frame_fields.data_len = 8;
    canMessageBlock.data.qword = 0x222;*/

    bool flag = CAN_tx(can1,&canMessageTransmittedBlock, 0);
}

void CANTransmissionReception::CANReception()
{
    if(CAN_rx(can1, &canMessageReceivedBlock, 0))
    {
        LE.on(1);
        if (!xQueueSend(receivedDataQueue, &canMessageReceivedBlock, 0))
        {
            // unexpected led
            LE.on(2);
            LOG_ERROR("This should never happen");
        }
        printf("%x\n", canMessageReceivedBlock.msg_id);
    }
    else
    {
        LE.on(3);
    }

}

void CANTransmissionReception::CANSetupFilter()
{

}
