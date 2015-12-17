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
#include "lpc_sys.h"       //sys_reboot()
#include "source/can_transmission_reception.h"
#include "scheduler_task.hpp"
#include "queue.h"
#include "file_logger.h"

#include "gps.hpp"

#define TestingWithoutAndroid 0

bool BootReplySent = true; //Used as indication of system startup flag
bool reboot = 0; // Used to indicate the reboot status
uint8_t Received_Checkpoint_Count=0;
uint16_t Total_Distance_To_Travel;

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
    if(CAN_setup_filter(canMessagesFilterList , 8, NULL, 0, NULL, 0, NULL, 0))
        printf("CAN Filter Setup Success\n");
    //CAN_bypass_filter_accept_all_msgs();
    CAN_reset_bus(can1);



    // Create a queue which would send the data to wherever received
//    QueueHandle_t CAN_received_Data_Queue = xQueueCreate(50, sizeof(can_msg_t));
//    scheduler_task::addSharedObject("CANreceivedMessagesQueue", CAN_received_Data_Queue);

    QueueHandle_t Checkpoint_Queue = xQueueCreate(100, sizeof(coordinates));
    scheduler_task::addSharedObject("CheckpointsQueue", Checkpoint_Queue);

    /*Hard coded checkpoint are added to queue
    instead of adding checkpoints from CAN receiver section*/
#if TestingWithoutAndroid
    coordinates checkpoint1/*[10]*/;
    uint8_t counter;
    //float lat[6] = {37.336401,37.336504,37.336582,37.336668,37.336767,37.336880};
    //float lon[6] = {-121.881673,-121.881455,-121.881302,-121.881154,-121.880916,-121.880793};
    float lat = 37.334781;
    float lon = -121.880913;
    //for (counter=0;counter<6;counter++)
    {
        checkpoint1/*[counter]*/.latitude = lat/*[counter]*/;
        checkpoint1/*[counter]*/.longitude = lon/*[counter]*/;
    }

//    for(counter=0;counter<6;counter++)
    {
        if (!xQueueSend(Checkpoint_Queue, &checkpoint1/*[counter]*/, 0))
        {
            // unexpected led
            //LE.toggle(2);//if checkpoints are not sent on queue LED will toggle
            LOG_ERROR("Checkpoint Queue Send Failed");
        }
        else
        {
            Received_Checkpoint_Count++;
            LE.toggle(3);
        }
    }
    Total_Distance_To_Travel = 350;
#endif

}

//void CANTransmissionReception::
bool CANTransmit(uint32_t msg_id , uint8_t * data, uint32_t len)
{
    can_msg_t tx;
    uint8_t i;
    tx.msg_id = msg_id;
    tx.frame_fields.is_29bit = 0;
    tx.frame_fields.data_len = len;
    for (i=0;i<len;i++)
    {
        tx.data.bytes[i] = data[i];
    }

    /* Toggles when CAN transmission happens*/
    LE.toggle(4);
    //printf("Transmitted ID: %x\n",tx.msg_id);
    return(CAN_tx(can1,&tx,0));
}


//void CANTransmissionReception::
void CANReception()
{
    //LE.toggle(2);
    can_msg_t canMessageReceivedBlock;
    QueueHandle_t Checkpoint_Queue = scheduler_task::getSharedObject("CheckpointsQueue");

    while(CAN_rx(can1, &canMessageReceivedBlock, 0))
    {

        switch (canMessageReceivedBlock.msg_id)
        {
            case RKillMessageFromMaster:
                // Reboot the system if Kill message is received from master
                sys_reboot();
                break;

            case RCheckpointsFromAndroid:
                coordinates checkpoint;
                memcpy(&checkpoint,&canMessageReceivedBlock.data,sizeof(coordinates));
                if (!xQueueSend(Checkpoint_Queue, &checkpoint, 0))
                {
                    // unexpected led
                    //LE.toggle(2);//if checkpoints are not sent on queue LED will toggle
                    LOG_ERROR("Checkpoint Queue Send Failed");
                }
                else
                {
                    Received_Checkpoint_Count++;
                    LE.toggle(3);
                }
                break;

            case RMapResetFromAndroid:
                printf("Map Reset");
                xQueueReset(Checkpoint_Queue);
                LE.toggle(3);
                LE.off(3);
                break;

            case RBootRequestFromMaster:
                //if (BootReplySent == false)
                {
                    uint32_t bootData = BootReplyData;
                    CANTransmit(TBootReplyToMaster,(uint8_t*)&bootData,sizeof(bootData));
                    printf("Boot Reply Sent\n");
                    //CANTransmitBootReply();
                    BootReplySent = true;
                }
                break;

            case RTotalTravelDistanceFromAndroid:
                memcpy(&Total_Distance_To_Travel,&canMessageReceivedBlock.data,sizeof(Total_Distance_To_Travel));
                break;

            case RDestinationReached:
                xQueueReset(Checkpoint_Queue);
                LE.off(3);
                //This message will not be used
                break;
        }
    }
}

//void CANTransmissionReception::
//void CANSetupFilter()
//{
//
//}


/* Function Created before implementing common Can transmit Function
void CANTransmitBootReply()
{
    can_msg_t BootInfo;
    BootInfo.frame_fields.is_29bit = 0;
    BootInfo.frame_fields.data_len = 8;
    BootInfo.data.qword = 20153010;
    BootInfo.msg_id = TBootReplyToMaster;

    //LE.toggle(4); //To test if boot reply is sent only once
    CANTransmission(BootInfo);

}*/

//void CANMessageProcessing()
//{
//    can_msg_t canMessageReceivedBlock;
//    QueueHandle_t CAN_received_Data_Queue = scheduler_task::getSharedObject("CANreceivedMessagesQueue");
//
//
//
//    if (xQueueReceive(CAN_received_Data_Queue, &canMessageReceivedBlock, 0))
//        //        printf("%x\n", canMessageReceivedBlock.msg_id);
//    {
//
//
//        if (canMessageReceivedBlock.msg_id == RKillMessageFromMaster)
//        {
//            //Probably reboot the system
//        }
//        else if (canMessageReceivedBlock.msg_id == RCheckpointsFromAndroid)
//        {
//            //LE.toggle(1); // To test is this condition is executed and checkpoints are sent on Queue
//            coordinates checkpoint;
//            checkpoint.latitude = *((float*)&canMessageReceivedBlock.data.dwords[0]);
//            checkpoint.longitude = *((float*)&canMessageReceivedBlock.data.dwords[1]);
//            if (!xQueueSend(Checkpoint_Queue, &checkpoint, 0))
//            {
//                // unexpected led
//                LE.toggle(2);//if checkpoints are not sent on queue LED will toggle
//                LOG_ERROR("Checkpoint Queue Send Failed");
//            }
//            else
//            {
//                Received_Checkpoint_Count++;
//                LE.toggle(3);
//            }
//        }
//        else if ((canMessageReceivedBlock.msg_id == RBootRequestFromMaster) && (BootReplySent == 0))
//        {
//            uint32_t bootData = BootReplyData;
//            CANTransmit(TBootReplyToMaster,(uint8_t*)&bootData,sizeof(bootData));
//            //CANTransmitBootReply();
//            BootReplySent = 1;
//        }
//        else if (canMessageReceivedBlock.msg_id == RTotalTravelDistanceFromAndroid)
//        {
//
//        }
//    }
//}

/*/* Function Created before implementing common Can transmit Function
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

    //printf("Sent Latitude: %x\n",Transmit_Coordinates.data.dwords[0]);
    //printf("Sent Longitude: %x\n",Transmit_Coordinates.data.dwords[1]);
    CANTransmission(Transmit_Coordinates);
}*/

