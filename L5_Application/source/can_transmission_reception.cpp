/*
 * can_transmission_reception.cpp
 *
 *  Created on: Oct 24, 2015
 *      Author: Gaurao Chaudhari
 */


#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "can.h"
#include "io.hpp"
#include "can_communication_ids.h"
#include "source/can_transmission_reception.h"
#include "scheduler_task.hpp"
#include "queue.h"
#include "file_logger.h"

#include "gps.hpp"

bool BootReplySent; //Used as indication of system startup flag
uint8_t Received_Checkpoint_Count;

//CANTransmissionReception::
//CANTransmissionReception()
//{
//    if(receivedDataQueue == NULL)
//        receivedDataQueue = xQueueCreate(50, sizeof(can_msg_t));
//}

//void BusOffCb(uint32_t param)
//{
//    printf("BusOffCb : Error");
//    CAN_reset_bus(can1);
//}

//void DataOverCanBuffer(uint32_t param)
//{
//    printf("DataOverCanBuffer : Error");
//    CAN_reset_bus(can1);
//}

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

    QueueHandle_t Checkpoint_Queue = xQueueCreate(100, sizeof(coordinates));
    scheduler_task::addSharedObject("CheckpointsQueue", Checkpoint_Queue);
}

//void CANTransmissionReception::
void CANTransmission(can_msg_t canMessageTransmittedBlock)
{
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
        //LE.toggle(1);
        LE.toggle(3);
        if (!xQueueSend(CAN_received_Data_Queue, &canMessageReceivedBlock, 0))
        {
            // unexpected led
            //LE.toggle(2);
            LOG_ERROR("This should never happen");
        }
//        printf("%x\n", canMessageReceivedBlock.msg_id);
    }
    else
    {
        //LE.toggle(3);
    }

}

//void CANTransmissionReception::
//void CANSetupFilter()
//{
//
//}

void CANTransmitBootReply()
{
    can_msg_t BootInfo;
    BootInfo.frame_fields.is_29bit = 0;
    BootInfo.frame_fields.data_len = 8;
    BootInfo.data.qword = 20153010;
    BootInfo.msg_id = TBootReplyToMaster;

    //LE.toggle(4); //To test if boot reply is sent only once
    CANTransmission(BootInfo);

}

void CANMessageProcessing()
{
    can_msg_t canMessageReceivedBlock;
    QueueHandle_t CAN_received_Data_Queue = scheduler_task::getSharedObject("CANreceivedMessagesQueue");

    QueueHandle_t Checkpoint_Queue = scheduler_task::getSharedObject("CheckpointsQueue");

    if (xQueueReceive(CAN_received_Data_Queue, &canMessageReceivedBlock, 0))
        //        printf("%x\n", canMessageReceivedBlock.msg_id);
    {
        if (canMessageReceivedBlock.msg_id == RKillMessageFromMaster)
        {
            //Probably reboot the system
        }
        else if (canMessageReceivedBlock.msg_id == RCheckpointsFromAndroid)
        {
            //LE.toggle(1); // To test is this condition is executed and checkpoints are sent on Queue
            coordinates checkpoint;
            checkpoint.latitude = *((float*)&canMessageReceivedBlock.data.dwords[0]);
            checkpoint.longitude = *((float*)&canMessageReceivedBlock.data.dwords[1]);
            if (!xQueueSend(Checkpoint_Queue, &checkpoint, 0))
            {
                // unexpected led
                LE.toggle(2);//if checkpoints are not sent on queue LED will toggle
                LOG_ERROR("Checkpoint Queue Send Failed");
            }
            else
                Received_Checkpoint_Count++;
        }
        else if ((canMessageReceivedBlock.msg_id == RBootRequestFromMaster) && (BootReplySent == 0))
        {
            CANTransmitBootReply();
            BootReplySent = 1;
        }
        else if (canMessageReceivedBlock.msg_id == RTotalTravelDistanceFromAndroid)
        {

        }
    }
}

void CANTransmitCoordinates(coordinates current_coordinates)
{
    can_msg_t Transmit_Coordinates;

    Transmit_Coordinates.frame_fields.is_29bit = 0;
    Transmit_Coordinates.frame_fields.data_len = 8;
//    printf("In Latitude: %f\n",current_coordinates.latitude);
//    printf("In Longitude: %f\n",current_coordinates.longitude);
    //memcpy(&Transmit_Coordinates.data,&current_coordinates,sizeof(current_coordinates));
    Transmit_Coordinates.data.dwords[0] = *((uint32_t*)&current_coordinates.latitude);
    Transmit_Coordinates.data.dwords[1] = *((uint32_t*)&current_coordinates.longitude);
    Transmit_Coordinates.msg_id = TSourceCoordinates;

    LE.toggle(1);// To test if coordinates are sent over CAN

    printf("Sent Latitude: %x\n",Transmit_Coordinates.data.dwords[0]);
    printf("Sent Longitude: %x\n",Transmit_Coordinates.data.dwords[1]);
    CANTransmission(Transmit_Coordinates);
}

