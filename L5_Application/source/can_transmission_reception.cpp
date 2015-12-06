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
#include "L4_IO/fat/fatfs_time.c"
//#include "source/cmd_handlers/handlers.cpp"
//#include "L5_Application/handlers.hpp"
#include "source/can_communication_ids.h"

#include "rtc.h"
//#include "L2_Drivers/src/rtc.c"
bool BootReplySent;//Used as indication of system startup flag
uint8_t bootData=BootReplyData;
rtc_t t;
char a1[6],a2[6],a3[6],a4[3],a5[3],a6[3],a7[3],a8[10],a11[5];
uint8_t sensor_data[4];
//Can_msg Message;

char sensor_front[50];
char sensor_left[50];
char sensor_right[50];
char sensor_rear[50];

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
    can_msg_t rx_msg;
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

        if(canMessageReceivedBlock.msg_id==0x210)
        {
           printf("received msg id 210\n");
        }
        switch (canMessageReceivedBlock.msg_id)
        {
            /*case RSourceFromGeo:

                //if(rec_bluetooth=='g'){

                    memcpy(&checkpoint,&canMessageReceivedBlock.data,sizeof(coordinates));
                    sprintf(source_coordinates,"%9.6f,%9.6f", checkpoint.latitude,checkpoint.longitude);
                    //printf("%9.6f\n",checkpoint.latitude);
                    u3.put(source_coordinates);
                //}

                break;*/

            case RBatteryStatFromMotor:
                //Send battery status to android
                break;

            case RBootReqFromMaster:
                //Send boot reply to Master after receiving message
                printf("here");
                if(BootReplySent==0)
                                    {
                                      //uint8_t bootData=BootReplyData;
                                      CANTransmit(TBootReplyToMaster,(uint8_t*)&bootData,sizeof(bootData));
                                      printf("Received Request from master= %x\n",canMessageReceivedBlock.msg_id);
                                      BootReplySent=1;
                                    }
                printf("Received Request from master= %x\n",canMessageReceivedBlock.msg_id);
                CANTransmit(TBootReplyToMaster,(uint8_t*)&bootData,sizeof(bootData));
                //CMD_HANDLER_FUNC(timeHandler);
                //const char* rtc_get_date_time_str(void);
             // rtc_settime(&t);
               //int rtc_gettime (void);


                //long t= get_fattime();
             printf("%s", rtc_get_date_time_str());
            // printf("%d",t);


                break;

            case RBootStatReqFromMaster:
                 //Send boot status from Master to Android for re-alinging user controls
                printf("Boot status");
                   u3.put("All controllers are ready");

                break;

            case RDestReachedFromMaster:
                //Send to Android for displaying destination reached message
                printf("Destination reached");//need to send to android through uart
                u3.put("Destination reached");
                break;

            case RSensorDataFromSensors:
                //Receive sensor data and send it to Android
                printf("in sensor");
                //Message.canReceivedMsg_Sensor=canMessageReceivedBlock;
                               printf("Received Request from master= %x\n",canMessageReceivedBlock.msg_id);
                         SensorData receivedSensorData;

                         receivedSensorData.FrontDistance =96/*canMessageReceivedBlock.data.bytes[0]*/;
                         receivedSensorData.LeftDistance =43/*canMessageReceivedBlock.data.bytes[1]*/;
                         receivedSensorData.RightDistance =58/*canMessageReceivedBlock.data.bytes[2]*/;
                         receivedSensorData.RearDistance =35/*canMessageReceivedBlock.data.bytes[3]*/;

                     sprintf(sensor_front,"f%d#",receivedSensorData.FrontDistance);

                     sprintf(sensor_left,"l%d#",receivedSensorData.LeftDistance);

                     sprintf(sensor_right,"s%d#",receivedSensorData.RightDistance);

                     sprintf(sensor_rear,"r%d#",receivedSensorData.RearDistance);

                     printf(" sensor F is %d\n",receivedSensorData.FrontDistance);

                     printf(" sensor L is %d\n",receivedSensorData.LeftDistance);

                     printf(" sensor R is %d\n",receivedSensorData.RightDistance);

                     printf(" sensor Rear is %d\n",receivedSensorData.RearDistance);

                      //u3.put("F:");
                      u3.put(sensor_front);

                      //u3.put("B:");
                      u3.put(sensor_left);

                      //u3.put("L:");
                      u3.put(sensor_right);

                      //u3.put("R:");
                      u3.put(sensor_rear);
                break;

            case RKillFromMaster:
                //Perform system reboot after receiving message
                sys_reboot();

                break;

        }

    }

}


