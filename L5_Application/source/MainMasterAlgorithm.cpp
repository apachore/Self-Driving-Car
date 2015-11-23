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
//#include "motorDrive.cpp"

#define straightSensorDangerDistance    50
#define turnSensorDangerDistance        30
#define lowestDangerDistance            20

#define fDanger                 40
#define fLow                    70
#define fLevel1                 130
#define fLevel2                 190

#define lDanger                 50
#define lLow                    50              // Need to look at the levels carefully
#define lLevel1                 100
#define lLevel2                 150

#define rDanger                 50
#define rLow                    50
#define rLevel1                 100
#define rLevel2                 150


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

void SensorProcessingAlgorithm(SensorData receivedSensorData)
{
    uint8_t levelOfSpeed = 0, levelOfDirection = 0;
    bool frontMotor = false;
    bool reverseMotor = false;
    bool leftMotor = false;
    bool rightMotor = false;
    bool brakeFlag = false;

    //printf("%d  %d  %d  %d\n", receivedSensorData.FrontDistance, receivedSensorData.LeftDistance, receivedSensorData.RightDistance, receivedSensorData.RearDistance);

    bool F1 = (receivedSensorData.FrontDistance < fLow) ? true : false;
    bool F2 = (receivedSensorData.FrontDistance < fLevel1 && receivedSensorData.FrontDistance > fLow) ? true : false;
    bool F3 = (receivedSensorData.FrontDistance < fLevel2 && receivedSensorData.FrontDistance > fLevel1) ? true : false;

    bool LDanger = (receivedSensorData.LeftDistance < lDanger) ? true : false;
    bool L1 = (receivedSensorData.LeftDistance < lLow) ? true : false;
    bool L2 = (receivedSensorData.LeftDistance < lLevel1 && receivedSensorData.LeftDistance > lLow) ? true : false;
    bool L3 = (receivedSensorData.LeftDistance < lLevel2 && receivedSensorData.LeftDistance > lLevel1) ? true : false;

    bool RDanger = (receivedSensorData.RightDistance < rDanger) ? true : false;
    bool Rt1 = (receivedSensorData.RightDistance < rLow) ? true : false;
    bool Rt2 = (receivedSensorData.RightDistance < rLevel1 && receivedSensorData.RightDistance > rLow) ? true : false;
    bool Rt3 = (receivedSensorData.RightDistance < rLevel2 && receivedSensorData.RightDistance > rLevel1) ? true : false;

    bool Reverse = (receivedSensorData.RearDistance < 60) ? true : false;

    if(!F1 && !F2 && !L1 && !L2 && !Rt1 && !Rt2) {
        frontMotor = true;
        levelOfSpeed = SpeedLevel3;
    }
    else if (F1) {
        LD.setLeftDigit('F');
        LD.setRightDigit('1');
        LE.toggle(1);
        if (!Reverse) {
            LE.toggle(4);
            reverseMotor = true;
            levelOfSpeed = SpeedLevel2;
            if (receivedSensorData.LeftDistance > receivedSensorData.RightDistance) {
                rightMotor = true;
                levelOfDirection = DirectionLevel5;
                LD.setRightDigit('R');
            }
            else if (receivedSensorData.RightDistance > receivedSensorData.LeftDistance) {
                leftMotor = true;
                levelOfDirection = DirectionLevel5;
                LD.setRightDigit('L');
            }
        }
    }
    else if (F2) {
        LD.setLeftDigit('F');
        LD.setRightDigit('2');
        LE.toggle(1);
        frontMotor = true;
        levelOfSpeed = SpeedLevel2;
        if (L1 && !Rt1 /*&& !Rt2*/) {
            // Extreme Right;
            LE.toggle(2);
            rightMotor = true;
            levelOfDirection = DirectionLevel4;
            levelOfSpeed = SpeedLevel1;
            LD.setRightDigit('R');
        }
        else if (L2 && !Rt1/* && !Rt2*/) {
            // Less level of right
            LE.toggle(2);
            rightMotor = true;
            levelOfDirection = DirectionLevel2;
            levelOfSpeed = SpeedLevel2;
            LD.setRightDigit('R');
        }
        else if(LDanger && !Reverse) {
            LE.toggle(4);
            frontMotor = false;
            reverseMotor = true;
            leftMotor = true;
            levelOfSpeed = SpeedLevel2;
            levelOfDirection = DirectionLevel5;
        }

        else if (Rt1 && !L1/* && !L2*/) {
            // Extreme Left;
            LE.toggle(3);
            leftMotor = true;
            levelOfDirection = DirectionLevel4;
            levelOfSpeed = SpeedLevel1;
            LD.setRightDigit('L');
        }
        else if (Rt2 && !L1 /*&& !L2*/) {
            // Less level of left;
            LE.toggle(3);
            leftMotor = true;
            levelOfDirection = DirectionLevel2;
            levelOfSpeed = SpeedLevel2;
            LD.setRightDigit('L');
        }
        else if(RDanger && !Reverse) {
            LE.toggle(4);
            frontMotor = false;
            reverseMotor = true;
            rightMotor = true;
            levelOfSpeed = SpeedLevel2;
            levelOfDirection = DirectionLevel5;
        }
    }
    //XXX: Have some low level where you wont be able to go ahead in any case and then stop or take a right.
    else if (L1 || L2) {
        LE.toggle(2);
        frontMotor = true;
        if (L1 && !Rt1 && !Rt2) {
            // Extreme right;
            rightMotor = true;
            levelOfDirection = DirectionLevel4;
            levelOfSpeed = SpeedLevel1;
            LD.setRightDigit('R');
            LD.setRightDigit('1');
        }
        else if (L2 && !Rt1 && !Rt2) {
            // Less level of right;
            rightMotor = true;
            levelOfDirection = DirectionLevel2;
            levelOfSpeed = SpeedLevel2;
            LD.setRightDigit('R');
            LD.setRightDigit('2');
        }
        else if (LDanger){
            frontMotor = false;
            reverseMotor = true;
            rightMotor = true;
            levelOfSpeed = SpeedLevel2;
            levelOfDirection = DirectionLevel4;
        }
    }
    //XXX: Have some low level where you wont be able to go ahead in any case and then stop or take a right.
    else if (Rt1 || Rt2) {
        LE.toggle(3);
        frontMotor = true;
        levelOfSpeed = 1;
        if (Rt1 && !L1 && !L2) {
            // Extreme left;
            leftMotor = true;
            levelOfDirection = DirectionLevel4;
            levelOfSpeed = SpeedLevel1;
        }
        else if (Rt2 && !L1 && !L2) {
            // Less level of left;
            leftMotor = true;
            levelOfDirection = DirectionLevel2;
            levelOfSpeed = SpeedLevel2;
        }
        else if (RDanger){
            frontMotor = false;
            reverseMotor = true;
            leftMotor = true;
            levelOfSpeed = SpeedLevel2;
            levelOfDirection = DirectionLevel4;
        }
    }
    if(F3 || L3 || Rt3)
    {
        //frontMotor = true;
        levelOfSpeed = SpeedLevel2;
    }

    MotorDriveFromSensors(frontMotor, reverseMotor, leftMotor, rightMotor, brakeFlag, levelOfSpeed, levelOfDirection);
}

void GeoProcessingAlgorithm(can_msg_t canData)
{

}

void DecisionAlgorithm(can_msg_t canReceivedData)
{


    //XXX Take Sensor data continuously and take the decisions at this end.
    //XXX whenever you think it is under a danger limit, slow it down and then take further decisions
    //XXX
    if(canReceivedData.msg_id == RSensorDataFromSensor)
    {

    }
    else
    {
        MotorDriveFromSensors(true, false, false, false, false, 1, 1);
    }
}

void SendKillMessageToAllControllers()
{
    can_msg_t canMessage;
    canMessage.msg_id = TKillMessageToAll;
    canMessage.frame_fields.is_29bit = 0;
    canMessage.frame_fields.data_len = 0;
    CANTransmission(canMessage);
}

void SystemInitialization()
{
    can_msg_t canMessage;
    canMessage.msg_id = TBootRequestToAll;
    canMessage.frame_fields.is_29bit = 0;
    canMessage.frame_fields.data_len = 0;
    CANTransmission(canMessage);
}

void AndroidReceivingProcessing()
{

}

void GetBootReplyFromModule()
{

}

void GeoDecision(uint8_t angle,uint8_t turn)
{
 /* sensor_obstruction is the globally declared flag which is set by Sensor Algorithm to indicate Geo_decision Algorithm about the
    obstruction, if there is an obstruction Geo Algorithm is bypassed and Car moves according to Sensor Algorithm.*/
   uint8_t levelOfSpeed = 0, levelOfDirection = 0;
   bool frontMotor = false;
   bool leftMotor = false;
   bool rightMotor = false;
   bool reverseMotor = false;
   bool brakeFlag = false;
    if(1)    //(sensor_obstruction)
      {
          if(geoReceivedData.finalDistance>10)  //(distance check)
          {
             if(angle>10)
              {
                  if(angle>=90)
                    {
                       if(turn==0)
                       {
                          puts("\nturn left with level 3");       // Turn left or right according to the data received with level 3.
                          leftMotor=true;
                          frontMotor=true;
                          levelOfDirection=3;
                          levelOfSpeed=1;

                       }
                        else
                        {
                            puts("\nturn right with level 3");
                            rightMotor=true;
                            frontMotor=true;
                            levelOfDirection=3;
                            levelOfSpeed=1;
                        }
                    }
                  else if(angle>=45&&angle<90)
                    {
                       if(turn==0)
                       {
                          puts("\nturn left with level 2");       // Turn left or right according to the data received with level 2.
                          leftMotor=true;
                          frontMotor=true;
                          levelOfDirection=2;
                          levelOfSpeed=1;

                       }
                       else
                       {
                          puts("\nturn right with level 2");
                          rightMotor=true;
                          frontMotor=true;
                          levelOfDirection=2;
                          levelOfSpeed=1;

                       }
                    }
                  else
                     {
                       if(turn==0)
                       {
                          puts("\nturn left with level 1");       // Turn left or right according to the data received with level 1.
                          leftMotor=true;
                          frontMotor=true;
                          levelOfDirection=1;
                          levelOfSpeed=1;

                       }
                       else
                       {
                          puts("\nturn right with level 1");
                          rightMotor=true;
                          frontMotor=true;
                          levelOfDirection=1;
                          levelOfSpeed=1;

                       }
                     }
              }

              else
                 {
                    puts("\nGo straight");     //  go straight.
                    frontMotor=true;
                    levelOfSpeed=2;

                 }
          }
          else
                {
                  if(geoReceivedData.finalDistance==0)//(total_distance==0)
                  {
                      puts("\nStop the Car");        // Stop the Car;
                      brakeFlag=true;
                      levelOfSpeed=0;
                  }
                  else
                   {
                        puts("\nMove slowly");      // Move with lower speed;
                        levelOfSpeed=1;
                   }
                }

          MotorDriveFromSensors(frontMotor, reverseMotor, leftMotor, rightMotor, brakeFlag, levelOfSpeed, levelOfDirection);
      }
    //  cout<<endl;
}
