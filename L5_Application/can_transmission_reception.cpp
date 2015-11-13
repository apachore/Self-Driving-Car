#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "can.h"
#include "io.hpp"

#include "can_transmission_reception.h"
#include "scheduler_task.hpp"
#include "queue.h"
#include "file_logger.h"

#include "compass/compass.hpp"

/*bool BootReplySent; //Used as indication of system startup flag
uint8_t Received_Checkpoint_Count;
uint16_t Total_Distance_To_Travel;*/

void CANInitialization()
{
    CAN_init(can1, 100, 500, 500, NULL, NULL);

    //Any message with 0xffff would disable the message
    if(CAN_setup_filter(canMessagesFilterList , 6, NULL, 0, NULL, 0, NULL, 0))

    //CAN_bypass_filter_accept_all_msgs();
    CAN_reset_bus(can1);

    // Create a queue which would send the data to wherever received
//    QueueHandle_t CAN_received_Data_Queue = xQueueCreate(50, sizeof(can_msg_t));
//    scheduler_task::addSharedObject("CANreceivedMessagesQueue", CAN_received_Data_Queue);

   // QueueHandle_t Turnangle_Queue = xQueueCreate(100, sizeof(int));
    //scheduler_task::addSharedObject("TurnAngleQueue", Turnangle_Queue);
}

//void CANTransmissionReception::
/*bool CANTransmit(uint32_t msg_id , uint8_t * data, uint32_t len)
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

    //printf("Transmitted ID: %x\n",tx.msg_id);
    return(CAN_tx(can1,&tx,0));
}*/

//void CANTransmissionReception::
/*void CANReception()
{
    //LE.toggle(2);
    can_msg_t canMessageReceivedBlock;
    QueueHandle_t Checkpoint_Queue = scheduler_task::getSharedObject("CheckpointsQueue");

    if(CAN_rx(can1, &canMessageReceivedBlock, 0))
    {

        switch (canMessageReceivedBlock.msg_id)
        {
            case RKillMessageFromMaster:
                break;

            case RCheckpointsFromAndroid:
                coordinates checkpoint;
                memcpy(&checkpoint,&canMessageReceivedBlock.data,sizeof(coordinates));
                if (!xQueueSend(Checkpoint_Queue, &checkpoint, 0))
                {
                    // unexpected led
                    LE.toggle(2);//if checkpoints are not sent on queue LED will toggle
                    LOG_ERROR("Checkpoint Queue Send Failed");
                }
                else
                {
                    Received_Checkpoint_Count++;
                    LE.toggle(3);
                }
                break;

           case RBootRequestFromMaster:
                if (BootReplySent == 0)
                {
                    uint32_t bootData = BootReplyData;
                    CANTransmit(TBootReplyToMaster,(uint8_t*)&bootData,sizeof(bootData));
                    //CANTransmitBootReply();
                    BootReplySent = 1;
                }
                //break;

            case RTotalTravelDistanceFromAndroid:
                memcpy(&Total_Distance_To_Travel,&canMessageReceivedBlock.data,sizeof(Total_Distance_To_Travel));
                break;
        }
    }
}
*/
