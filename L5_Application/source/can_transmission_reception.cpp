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

#include "source/can_transmission_reception.h"
#include "scheduler_task.hpp"
#include "queue.h"
#include "file_logger.h"


void CANInitialization()
{
    CAN_init(can1, 100, 30, 30, NULL, NULL);

    if(CAN_setup_filter(canMessagesFilterList , 2, NULL, 0, NULL, 0, NULL, 0))
    CAN_reset_bus(can1);

}


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

    printf("Transmitted ID: %x\n",tx.msg_id);
    return(CAN_tx(can1,&tx,0));
}

//void CANTransmissionReception::
void CANReception()
{
    coordinates checkpoint;
    //LE.toggle(2);
    can_msg_t canMessageReceivedBlock;
    //QueueHandle_t Checkpoint_Queue = scheduler_task::getSharedObject("CheckpointsQueue");

    if(CAN_rx(can1, &canMessageReceivedBlock, 0))
    {

        switch (canMessageReceivedBlock.msg_id)
        {
            case RSourceFromGeo:

                memcpy(&checkpoint,&canMessageReceivedBlock.data,sizeof(coordinates));

                break;

        }
    }
}



