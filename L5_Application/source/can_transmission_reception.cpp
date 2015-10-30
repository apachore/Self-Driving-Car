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


//CANTransmissionReception::
//CANTransmissionReception()
//{
//    if(receivedDataQueue == NULL)
//        receivedDataQueue = xQueueCreate(50, sizeof(can_msg_t));
//}

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

//void CANTransmissionReception::
void CANInitialization()
{
    CAN_init(can1, 100, 30, 30, NULL, NULL);

    //Any message with 0xffff would disable the message
    if(CAN_setup_filter(canMessagesFilterList , 6, NULL, 0, NULL, 0, NULL, 0))
        printf("CAN Filter Setup Success\n");
    //CAN_bypass_filter_accept_all_msgs();
    CAN_reset_bus(can1);

    // Create a queue which would send the data to wherever received
    QueueHandle_t CAN_received_Data_Queue = xQueueCreate(50, sizeof(can_msg_t));
    scheduler_task::addSharedObject("CANreceivedMessagesQueue", CAN_received_Data_Queue);
}

//void CANTransmissionReception::
void CANTransmission(/*can_msg_t canMessageBlock*/)
{
    can_msg_t canMessageTransmittedBlock;
/*    canMessageBlock.msg_id = 0x2;
    canMessageBlock.frame_fields.data_len = 8;
    canMessageBlock.data.qword = 0x222;*/

    bool flag = CAN_tx(can1,&canMessageTransmittedBlock, 0);
}

//void CANTransmissionReception::
void CANReception()
{
    //LE.toggle(4);
    can_msg_t canMessageReceivedBlock;
    QueueHandle_t CAN_received_Data_Queue = scheduler_task::getSharedObject("CANreceivedMessagesQueue");
    if(CAN_rx(can1, &canMessageReceivedBlock, 0))
    {
        LE.toggle(1);
        if (!xQueueSend(CAN_received_Data_Queue, &canMessageReceivedBlock, 0))
        {
            // unexpected led
            LE.toggle(2);
            LOG_ERROR("This should never happen");
        }
//        printf("%x\n", canMessageReceivedBlock.msg_id);
    }
    else
    {
        LE.toggle(3);
    }

}

//void CANTransmissionReception::CANSetupFilter()
//{
//
//}
