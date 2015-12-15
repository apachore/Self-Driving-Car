/*
 * MainMasterAlgorithm.h
 *
 *  Created on: Oct 24, 2015
 *      Author: Gaurav
 */

#ifndef L5_APPLICATION_SOURCE_MAINMASTERALGORITHM_H_
#define L5_APPLICATION_SOURCE_MAINMASTERALGORITHM_H_


#include "stdio.h"
#include "string.h"
#include "scheduler_task.hpp"
#include "can.h"
#include "can_communication_ids.h"
#include "lpc_sys.h"


void MotorDriveFromSensors(bool frontMotor, bool reverseMotor, bool leftMotor, bool rightMotor, bool brakeFlag, uint8_t levelOfSpeed, uint8_t levelOfDirection);
void SensorProcessingAlgorithm();
void SendKillMessageToAllControllers();
void GeoDecision();
bool SendBootRequest();
void GetBootReplyFromModuleCheck();
void KillTask();  // Call back function for On-board Kill switch Implementation.

#endif /* L5_APPLICATION_SOURCE_MAINMASTERALGORITHM_H_ */
