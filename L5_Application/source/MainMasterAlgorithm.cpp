/*
 * This file has all the Master Algorithm needed to drive the car.
 * To drive the car, this file has algorithm for detecting sensor data and taking
 * appropriate decision.
 *
 * Authored : Gaurao Chaudhari
 * Date     : 10/18/2015
 */

#include "stdio.h"
#include "string.h"
#include "MainMasterAlgorithm.h"
#include "io.hpp"
#include "can_communication_ids.h"
#include "can_transmission_reception.h"
#include "file_logger.h"
#include "utilities.h"

extern GeoDistanceData receivedDistanceData;
extern GeoTurnData receivedTurnData;
extern SensorData receivedSensorData;
extern bool isSensorObstruction;
bool destinationReached = false;
extern bool bootFromAndroid;
extern bool bootFromGeo;
extern bool bootFromMotorIO;
extern bool bootFromSensor;
extern bool bootRepliesReceived;
bool stopFromKill = false;


#define destinationReachedDistance    20

#define fDanger                 40
#define fLow                    40
#define fLevel1                 130
#define fLevel2                 190

#define lDanger                 40
#define lLow                    70              // Need to look at the levels carefully
#define lLevel1                 120
#define lLevel2                 150

#define rDanger                 40
#define rLow                    70      //
#define rLevel1                 120     // 100
#define rLevel2                 150

#define reverseDistance         60

void KillTask()
{
    printf("Here");
    stopFromKill = true;
    SendKillMessageToAllControllers();
}

void MotorDriveFromSensors(bool frontMotor, bool reverseMotor, bool leftMotor, bool rightMotor, bool brakeFlag, uint8_t levelOfSpeed, uint8_t levelOfDirection)
{
    can_msg_t canMessage;

    uint32_t msgId = TMotorControlToMotor;
    canMessage.msg_id = TMotorControlToMotor;
    canMessage.frame_fields.is_29bit = 0;
    canMessage.frame_fields.data_len = 4;

    canMessage.data.bytes[0] = 0x0;
    canMessage.data.bytes[1] = 0x0;
    canMessage.data.bytes[2] = 0x0;
    canMessage.data.bytes[3] = 0x0;

    if(frontMotor)
    {
        canMessage.data.bytes[2] = 0x1;
        canMessage.data.bytes[3] = levelOfSpeed;
    }
    if(reverseMotor)
    {
        canMessage.data.bytes[2] = 0x2;
        canMessage.data.bytes[3] = levelOfSpeed;
    }
    if(leftMotor)
    {
        canMessage.data.bytes[0] = 0x1;
        canMessage.data.bytes[1] = levelOfDirection;
    }
    if(rightMotor)
    {
        canMessage.data.bytes[0] = 0x2;
        canMessage.data.bytes[1] = levelOfDirection;
    }

    printf("%d  %d  %d  %d\n", canMessage.data.bytes[0], canMessage.data.bytes[1], canMessage.data.bytes[2], canMessage.data.bytes[3]);
    CANTransmission(canMessage);
    //CANTransmission(canMessage.msg_id, &canMessage.data.bytes[0], 4);
}

void SensorProcessingAlgorithm()
{
    uint8_t levelOfSpeed = 0, levelOfDirection = 0;
    bool frontMotor = false;
    bool reverseMotor = false;
    bool leftMotor = false;
    bool rightMotor = false;
    bool brakeFlag = false;

    bool F1 = (receivedSensorData.FrontDistance < fLow) ? true : false;
    bool F2 = (receivedSensorData.FrontDistance < fLevel1 && receivedSensorData.FrontDistance >= fLow) ? true : false;
    bool F3 = (receivedSensorData.FrontDistance < fLevel2 && receivedSensorData.FrontDistance >= fLevel1) ? true : false;

    bool LDanger = (receivedSensorData.LeftDistance < lDanger) ? true : false;
    bool L1 = (receivedSensorData.LeftDistance < lLow && receivedSensorData.LeftDistance >= lDanger) ? true : false;
    bool L2 = (receivedSensorData.LeftDistance < lLevel1 && receivedSensorData.LeftDistance >= lLow) ? true : false;
    bool L3 = (receivedSensorData.LeftDistance < lLevel2 && receivedSensorData.LeftDistance >= lLevel1) ? true : false;

    bool RtDanger = (receivedSensorData.RightDistance < rDanger) ? true : false;
    bool Rt1 = (receivedSensorData.RightDistance < rLow && receivedSensorData.RightDistance >= rDanger) ? true : false;
    bool Rt2 = (receivedSensorData.RightDistance < rLevel1 && receivedSensorData.RightDistance >= rLow) ? true : false;
    bool Rt3 = (receivedSensorData.RightDistance < rLevel2 && receivedSensorData.RightDistance >= rLevel1) ? true : false;

    bool Reverse = (receivedSensorData.RearDistance < reverseDistance) ? true : false;

    if(F1 || F2 || F3 || LDanger || L1 || L2 || L3 || RtDanger || Rt1 || Rt2 || Rt3) {
        isSensorObstruction = true;
    }
    else {
        isSensorObstruction = false;     // Adjustment for now
        return;
    }
    if(destinationReached)
    {
        return;
    }

//    printf("%d  %d  %d  %d\n", receivedSensorData.FrontDistance, receivedSensorData.LeftDistance, receivedSensorData.RightDistance, receivedSensorData.RearDistance);

    if(!F1 && !F2 && !F3 && !LDanger && !L1 && !L2 && !L3 && !RtDanger && !Rt1 && !Rt2 && !Rt3) {

        frontMotor = true;
        levelOfSpeed = SpeedLevel2;
    }
    else if (F1) {
/*        LD.setLeftDigit('F');
        LD.setRightDigit('1');*/
        LE.toggle(1);
        if (!Reverse) {
            LE.toggle(4);
            reverseMotor = true;
            levelOfSpeed = SpeedLevel1;
            if (receivedSensorData.LeftDistance > receivedSensorData.RightDistance) {
                rightMotor = true;
                levelOfDirection = DirectionLevel5;
                //LD.setRightDigit('R');
            }
            else if (receivedSensorData.RightDistance > receivedSensorData.LeftDistance) {
                leftMotor = true;
                levelOfDirection = DirectionLevel5;
                //LD.setRightDigit('L');
            }
        }
    }
    else if (F2) {
        /*LD.setLeftDigit('F');
        LD.setRightDigit('2');*/
        LE.toggle(1);
        frontMotor = true;
        levelOfSpeed = SpeedLevel2;
        ///XXX: Need to check again for this Rt2 needed or not?
        if (L1 && !RtDanger && !Rt1 /*&& !Rt2*/) {
            // Extreme Right;
            LE.toggle(2);
            frontMotor = true;
            rightMotor = true;
            levelOfDirection = DirectionLevel4;
            levelOfSpeed = SpeedLevel1;
            //LD.setRightDigit('R');
            //LD.setRightDigit('R');
        }
        ///XXX: // Need to check this again whether sharp direction is needed or not
        else if (L2 && !RtDanger && !Rt1/* && !Rt2*/) {
            // Less level of right
            LE.toggle(2);
            frontMotor = true;
            rightMotor = true;
            levelOfDirection = DirectionLevel2;
            levelOfSpeed = SpeedLevel2;
//            LD.setRightDigit('R');
        }
        else if(LDanger && !Reverse) {
            LE.toggle(4);
            frontMotor = false;
            reverseMotor = true;
            leftMotor = true;
            levelOfSpeed = SpeedLevel2;
            levelOfDirection = DirectionLevel5;
        }
        // XXX: If in F2 and L1 and R1 or less than that - Consider this case after testing. This will be a rare condition
        //  Still have a look at this.

        else if (Rt1 && !LDanger && !L1/* && !L2*/) {
            // Extreme Left;
            LE.toggle(3);
            frontMotor = true;
            leftMotor = true;
            levelOfDirection = DirectionLevel4;
            levelOfSpeed = SpeedLevel1;
//            LD.setRightDigit('L');
        }
        else if (Rt2 && !LDanger && !L1 /*&& !L2*/) {
            // Less level of left;
            LE.toggle(3);
            frontMotor = true;
            leftMotor = true;
            levelOfDirection = DirectionLevel2;
            levelOfSpeed = SpeedLevel2;
//            LD.setRightDigit('L');
        }
        else if(RtDanger && !Reverse) {
            LE.toggle(4);
            frontMotor = false;
            reverseMotor = true;
            rightMotor = true;
            levelOfSpeed = SpeedLevel2;
            levelOfDirection = DirectionLevel5;
        }
        //XXX: To check again (Written now)
        else if (LDanger && RtDanger && !Reverse) {
            reverseMotor = true;
            levelOfSpeed = SpeedLevel2;
            if (receivedSensorData.LeftDistance > receivedSensorData.RightDistance) {
                rightMotor = true;
                levelOfDirection = DirectionLevel5;
//                LD.setRightDigit('R');
            }
            else if (receivedSensorData.RightDistance > receivedSensorData.LeftDistance) {
                leftMotor = true;
                levelOfDirection = DirectionLevel5;
//                LD.setRightDigit('L');
            }
        }
    }
    //XXX: Have some low level where you wont be able to go ahead in any case and then stop or take a right.
    else if (L1 || L2 || LDanger) {
        LE.toggle(2);
        if (L1 && !RtDanger && !Rt1 /*&& !Rt2*/) {
            // Extreme right;
            frontMotor = true;
            rightMotor = true;
            levelOfDirection = DirectionLevel4;
            levelOfSpeed = SpeedLevel1;
            /*LD.setRightDigit('R');
            LD.setRightDigit('1');*/
        }
        else if (L2 && !RtDanger && !Rt1 /*&& !Rt2*/) {
            // Less level of right;
            frontMotor = true;
            rightMotor = true;
            levelOfDirection = DirectionLevel2;
            levelOfSpeed = SpeedLevel2;
         /*   LD.setRightDigit('R');
            LD.setRightDigit('2');*/
        }
        ///XXX : Added now. Check this condition in testing now.
        else if (Rt1 && !LDanger && !L1 /*&& !L2*/) {
            // Extreme left;
            frontMotor = true;
            leftMotor = true;
            levelOfDirection = DirectionLevel4;
            levelOfSpeed = SpeedLevel1;
        }
        else if (LDanger) {
            LE.toggle(4);
            frontMotor = false;
            reverseMotor = true;
            leftMotor = true;
            levelOfSpeed = SpeedLevel2;
            levelOfDirection = DirectionLevel4;
        }
    }
    //XXX: Have some low level where you wont be able to go ahead in any case and then stop or take a right.
    else if (Rt1 || Rt2 || RtDanger) {
        LE.toggle(3);
        levelOfSpeed = 1;
        if (Rt1 && !LDanger && !L1 /*&& !L2*/) {
            // Extreme left;
            frontMotor = true;
            leftMotor = true;
            levelOfDirection = DirectionLevel4;
            levelOfSpeed = SpeedLevel1;
        }
        else if (Rt2 && !LDanger && !L1 /*&& !L2*/) {
            // Less level of left;
            frontMotor = true;
            leftMotor = true;
            levelOfDirection = DirectionLevel2;
            levelOfSpeed = SpeedLevel2;
        }
        if (L1 && !RtDanger && !Rt1 /*&& !Rt2*/) {
            // Extreme right;
            frontMotor = true;
            rightMotor = true;
            levelOfDirection = DirectionLevel4;
            levelOfSpeed = SpeedLevel1;
            /*LD.setRightDigit('R');
            LD.setRightDigit('1');*/
        }
        else if (RtDanger){
            LE.toggle(4);
            frontMotor = false;
            reverseMotor = true;
            rightMotor = true;
            levelOfSpeed = SpeedLevel2;
            levelOfDirection = DirectionLevel4;
        }
    }
    else if(F3 || L3 || Rt3)
    {
        frontMotor = true;
        levelOfSpeed = SpeedLevel2;
    }
    MotorDriveFromSensors(frontMotor, reverseMotor, leftMotor, rightMotor, brakeFlag, levelOfSpeed, levelOfDirection);
}


bool SendBootRequest()
{
    delay_ms(3);                        // Delay to properly start all the controllers;
    can_msg_t bootRequestCANMessage;
    bootRequestCANMessage.msg_id = TBootRequestToAll;
    CANTransmission(bootRequestCANMessage);
    return true;
}

void SendKillMessageToAllControllers()
{
    can_msg_t canMessage;
    canMessage.msg_id = TKillMessageToAll;
    canMessage.frame_fields.is_29bit = 0;
    canMessage.frame_fields.data_len = 0;
    CANTransmission(canMessage);
}

void GetBootReplyFromModuleCheck()
{
    if(bootFromAndroid && bootFromGeo && bootFromMotorIO && bootFromSensor)
    {
        bootRepliesReceived = true;
    }
}

void SendBootStatusToAllControllers()
{
    can_msg_t canMessageBootStatus;
    canMessageBootStatus.msg_id = TBootStatusToAll;
    canMessageBootStatus.frame_fields.is_29bit = 0;
    canMessageBootStatus.frame_fields.data_len = 0;
    CANTransmission(canMessageBootStatus);
}

void GeoDecision(/*uint8_t turningAngle,uint8_t turnDirection*/)
{
    /* sensor_obstruction is the globally declared flag which is set by Sensor Algorithm to indicate Geo_decision Algorithm about the
    obstruction, if there is an obstruction GEO Algorithm is bypassed and Car moves according to Sensor Algorithm.*/

    uint8_t levelOfSpeed = 0, levelOfDirection = 0;
    bool frontMotor = false;
    bool leftMotor = false;
    bool rightMotor = false;
    bool reverseMotor = false;
    bool brakeFlag = true;

    uint16_t finalDistance = receivedDistanceData.FinalDistance;
    uint16_t nextCheckPointDistance = receivedDistanceData.NextCheckpointDistance;
    uint8_t turningAngle = receivedTurnData.TurningAngle;
    uint8_t turnDirection = receivedTurnData.TurnDirection;

//    destinationReached = nextCheckPointDistance < destinationReachedDistance ? true : false;

    LD.setNumber(nextCheckPointDistance);
    // Changed the flag from here to
    if(!isSensorObstruction)
    {
        printf("%d  %d  %d  %d\n", finalDistance, nextCheckPointDistance, turningAngle, turnDirection);
        if(nextCheckPointDistance > destinationReachedDistance)  //(distance check)
        {
            if(turningAngle > 15)
            {
                if(turningAngle >= 90)
                {
                    if(turnDirection == 1)
                    {
                        // Turn left with level 4
                        // Turn left or right according to the data received with level 4.
                        LE.toggle(2);
                        leftMotor = true;
                        frontMotor = true;
                        levelOfDirection = DirectionLevel4;
                        levelOfSpeed = SpeedLevel1;

                    }
                    else if(turnDirection == 2)
                    {
                        LE.toggle(3);
                        // Turn right with level 3
                        rightMotor = true;
                        frontMotor = true;
                        levelOfDirection = DirectionLevel4;
                        levelOfSpeed = SpeedLevel1;
                    }
                }
                else if(turningAngle >= 45 && turningAngle < 90)
                {
                    if(turnDirection == 1)
                    {
                        LE.toggle(2);
                        // Turn left with level 2
                        // Turn left or right according to the data received with level 2.
                        leftMotor = true;
                        frontMotor = true;
                        levelOfDirection = DirectionLevel2;
                        levelOfSpeed = SpeedLevel2;
                    }
                    else if(turnDirection == 2)
                    {
                        LE.toggle(3);
                        // Turn right with level 2
                        rightMotor = true;
                        frontMotor = true;
                        levelOfDirection = DirectionLevel2;
                        levelOfSpeed = SpeedLevel2;
                    }
                }
                else
                {
                    if(turnDirection == 1)
                    {
                        LE.toggle(2);
                        // Turn left with level 1");       // Turn left or right according to the data received with level 1.
                        leftMotor = true;
                        frontMotor = true;
                        levelOfDirection = DirectionLevel1;
                        levelOfSpeed = SpeedLevel2;
                        if(nextCheckPointDistance < 50)
                            levelOfSpeed = SpeedLevel1;
                    }
                    else if(turnDirection == 2)
                    {
                        LE.toggle(3);
                        // Turn right with level 1
                        rightMotor = true;
                        frontMotor = true;
                        levelOfDirection = DirectionLevel1;
                        levelOfSpeed = SpeedLevel2;
                        if(nextCheckPointDistance < 50)
                            levelOfSpeed = SpeedLevel1;
                    }
                }
            }
            else
            {
                LE.toggle(1);
                //  go straight.
                frontMotor = true;
                levelOfSpeed = SpeedLevel2;
                if(nextCheckPointDistance < 50)
                    levelOfSpeed = SpeedLevel1;
            }
        }
        else
        {
            /*if(finalDistance == 0)//(total_distance==0)
            {*/
                // Stop the Car;
                frontMotor = false;
                leftMotor = false;
                rightMotor = false;
                reverseMotor = false;
                levelOfDirection = 0;
                levelOfSpeed = 0;
            /*}
            else
            {
                LE.toggle(1);
                // Move with lower speed;
                frontMotor = true;
                levelOfSpeed = SpeedLevel1;
            }*/
        }
        MotorDriveFromSensors(frontMotor, reverseMotor, leftMotor, rightMotor, brakeFlag, levelOfSpeed, levelOfDirection);
    }

}
