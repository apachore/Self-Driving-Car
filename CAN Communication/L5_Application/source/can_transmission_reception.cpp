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
#include "lpc_sys.h"
bool BootReplySent;//Used as indication of system startup flag

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
    coordinates checkpoint;
    char source_coordinates[20];

    //temporary code for static source coordinates
    if(rec_bluetooth=='g'){

        checkpoint.latitude=27.12312;
        checkpoint.longitude=-121.12434;
        sprintf(source_coordinates,"la%9.6f,lo%9.6f",checkpoint.latitude,checkpoint.longitude);
        u3.put(source_coordinates);

    }


    if(CAN_rx(can1, &canMessageReceivedBlock, 0))
    {
        switch (canMessageReceivedBlock.msg_id)
        {
            case RSourceFromGeo:

                if(rec_bluetooth=='g'){

                    memcpy(&checkpoint,&canMessageReceivedBlock.data,sizeof(coordinates));
                    sprintf(source_coordinates,"%9.6f,%9.6f", checkpoint.latitude,checkpoint.longitude);
                    u3.put(source_coordinates);
                }

                break;

            case RBatteryStatFromMotor:
                //Send battery status to android
                break;

            case RBootReqFromMaster:
                //Send boot reply to Master after receiving message
                if(BootReplySent==0)
                                    {
                                      uint32_t bootData=BootReplyData;
                                      CANTransmit(TBootReplyToMaster,(uint8_t*)&bootData,sizeof(bootData));
                                      BootReplySent=1;
                                    }

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


