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


void SensorProcessingAlgorithm(can_msg_t canData);
void DecisionAlgorithm(can_msg_t canData);


#endif /* L5_APPLICATION_SOURCE_MAINMASTERALGORITHM_H_ */
