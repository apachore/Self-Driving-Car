/*
 * can_transmission_reception.h
 *
 *  Created on: Oct 24, 2015
 *      Author: Gaurao Chaudhari
 */

#ifndef L5_APPLICATION_SOURCE_CAN_TRANSMISSION_RECEPTION_H_
#define L5_APPLICATION_SOURCE_CAN_TRANSMISSION_RECEPTION_H_

#include "stdio.h"
#include "stdlib.h"
#include "can.h"
#include "io.hpp"

/* This function would initialize the CAN bus. It will setup the filter that is to be used for receiving
   messages. */
void CANInitialization();
void CANTransmission(/*can_msg_t canMessageBlock*/uint32_t msg_id, uint8_t* data, uint32_t length);
//void CANTransmission(can_msg_t canMessageBlock);
bool CANReception(can_msg_t& canMessageBlock);


#endif /* L5_APPLICATION_SOURCE_CAN_TRANSMISSION_RECEPTION_H_ */
