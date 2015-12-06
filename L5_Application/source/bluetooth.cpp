
#include <source/bluetooth.hpp>
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "can.h"
#include "io.hpp"

#include "source/can_transmission_reception.h"
#include "source/can_communication_ids.h"
#include "scheduler_task.hpp"
#include "queue.h"
#include "file_logger.h"

#include "uart3.hpp"
#include "uart_dev.hpp"


bluetoothTask::bluetoothTask(uint8_t priority) :scheduler_task("bluetooth", 2000,priority)
{

}

bool bluetoothTask::init(void) {

    Uart3& u3 = Uart3::getInstance();
    u3.init(9600,50,50);

    return true;
}

bool bluetoothTask::run(void *p)
{

    char recChar[1];
    char *parsed_coordinates[3];
    int i=0;
    coordinates checkpoint;

    Uart3& u3 = Uart3::getInstance();

    u3.gets(recChar,sizeof(recChar),1000);

    switch(recChar[0]){

        case 'a':
            CANTransmit(TStartToMaster,0,0);
                    printf("Message send %s\n", recChar);
                    recChar[0]='0';
            break;

        case 'b':
            CANTransmit(TStartToMaster,0,0);
                    printf("Message send %s\n", recChar);
                    recChar[0]='0';
            break;

        case 'c':
            CANTransmit(TStopToMaster,0,0);
                    printf("Message send %s\n", recChar);
                    recChar[0]='0';
            break;

        case 'd':
            CANTransmit(TKillToMaster,0,0);
                    printf("Message send %s\n", recChar);
                    recChar[0]='0';
            break;

        case 'f':
            char *token;

            token=strtok(recChar,",");
            while(token!=NULL){
                parsed_coordinates[i]=token;
                token=strtok(NULL,",");
                i++;
            }

            sscanf(parsed_coordinates[1],"%f",&checkpoint.latitude);
            sscanf(parsed_coordinates[2],"%f",&checkpoint.longitude);

            printf("%f\n",checkpoint.latitude);
            printf("%f\n",checkpoint.longitude);

            CANTransmit(TDestToGeo,(uint8_t*)&checkpoint,sizeof(checkpoint));
            recChar[0]='0';

            break;

        case 'g':

            CANReception(recChar[0],u3);
            recChar[0]='0';
            break;

        case 'h'://monitor
            CANReception(recChar[0],u3);
            recChar[0]='0';
             break;


    }

    vTaskDelay(100);

    return true;
}

