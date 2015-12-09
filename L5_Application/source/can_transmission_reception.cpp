#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "can.h"
#include "io.hpp"

#include "source/can_transmission_reception.h"
#include "scheduler_task.hpp"
#include "queue.h"
#include "file_logger.h"
#include "lpc_sys.h"

coordinates checkpoint;

void CANInitialization()
{
    CAN_init(can1, 100, 30, 30, NULL, NULL);

    if(CAN_setup_filter(canMessagesFilterList , 8, NULL, 0, NULL, 0, NULL, 0))
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

void CANReception(char rec_bluetooth,Uart3 u3){

    can_msg_t canMessageReceivedBlock;
    char source_coordinates1[50];
    char source_coordinates2[50];

    while(CAN_rx(can1, &canMessageReceivedBlock, 0))
    {
        switch (canMessageReceivedBlock.msg_id)
        {
            case RSourceFromGeo:

                memcpy(&checkpoint,&canMessageReceivedBlock.data,sizeof(coordinates));

                sprintf(source_coordinates1,"x%8.5f#",checkpoint.latitude);
                sprintf(source_coordinates2,"y%10.5f#",checkpoint.longitude);
                      u3.put(source_coordinates1);
                      u3.put(source_coordinates2);

                break;

            case RBatteryStatFromMotor:
                //Send battery status to android
                break;

            case RBootReqFromMaster:
                //Send boot reply to Master after receiving message
                break;

            case RBootStatReqFromMaster:
                 //Send boot status from Master to Android for re-alinging user controls
                break;

            case RDestReachedFromMaster:
                //Send to Android for displaying destination reached message
                break;

            case RSensorDataFromSensors:
                //Receive sensor data and send it to Android

                break;

            case RKillFromMaster:
                //Perform system reboot after receiving message
                sys_reboot();
                break;

        }

    }

}

