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
#include "MainMasterAlgorithm.h"
#include "file_logger.h"
#include "string.h"


// XXX: The callback is called from inside CAN Bus interrupt, should not use printf() here
// XXX: CAN Bus reset should not be called right away, it should reset maybe in 10Hz if can bus is off


static bool run = true;
static bool stop = false;
GeoTurnData receivedTurnData;
GeoDistanceData receivedDistanceData;
bool isSensorObstruction = false;


void DataOverCanBuffer(uint32_t param)
{
    LOG_ERROR("Data over CAN buffer");
}

void CANInitialization()
{
    CAN_init(can1, 100, 500, 500, NULL, *DataOverCanBuffer);
    CAN_reset_bus(can1);

    //Any message with 0xffff would disable the message
    CAN_setup_filter(canMessagesFilterList, 8, groupList, 2, NULL, 0, NULL, 0);
}



void CANTransmission(can_msg_t canMessageBlock/*uint32_t msg_id, uint8_t* data, uint32_t length*/)
{

    CAN_tx(can1,&canMessageBlock, 0);
}

bool CANReception(can_msg_t& canMessageBlock)
{
    bool receptionSuccessful = false;
    // XXX: Empty out all the messages in the queue
    // Empty all messages that have arrived within last 10ms
    bool sentStartFromAndroid = run && !stop;
    bool sentStopFromAndroid = !run && stop;
    while (CAN_rx(can1, &canMessageBlock, 0))
    {
        switch (canMessageBlock.msg_id)
        {
            case RSensorDataFromSensor:


            {
                if(sentStartFromAndroid)
                {
                    SensorData receivedSensorData;
                    memcpy(&receivedSensorData,&canMessageBlock.data,sizeof(SensorData));
                    SensorProcessingAlgorithm(receivedSensorData);
                }
                else
                {
                    MotorDriveFromSensors(false, false, false, false, false, 0, 0);
                }
                break;
            }

            case RDistanceFinalAndNextCheckpoint:

                //printf("%d   %d\n", geoReceivedData.FinalDistance, geoReceivedData.NextCheckpointDistance);
                memcpy(&receivedDistanceData,&canMessageBlock.data,sizeof(GeoDistanceData));

                break;

            case RHeadingAndBearingToGeo:
                //printf("%d   %d\n", geoReceivedData.TurningAngle, geoReceivedData.TurnDirection);
                memcpy(&receivedTurnData,&canMessageBlock.data,sizeof(GeoTurnData));

                break;


            case RRunAndPauseCommandFromAndroid:
                printf("Run received");
                MotorDriveFromSensors(false, false, false, false, false, 0, 0);
                run = true;
                stop = false;

                break;

            case RStopMessageFromAndroid:
                printf("Stop received");
                MotorDriveFromSensors(false, false, false, false, false, 0, 0);
                run = false;
                stop = true;
                break;

            case RKillMessageFromAndroid:
                SendKillMessageToAllControllers();
                break;

            case RBootReplyFromAndroid:
                GetBootReplyFromModule();
                break;

            case RBootReplyFromGeo:
                GetBootReplyFromModule();
                break;

            case RBootReplyFromMotor:
                GetBootReplyFromModule();
                break;

            case RBootReplyFromSensor:
                GetBootReplyFromModule();
                break;

                // XXX: Why do this in the default case?
            default:
                if(run)
                {
                    //MotorDriveFromSensors(true, false, false, false, false, SpeedLevel3, 0);
                }
                else if(stop)
                {
                    //MotorDriveFromSensors(false, false, false, false, false, 0, 0);
                }
                break;
        }
        receptionSuccessful = true;
    }
    return receptionSuccessful;
}
