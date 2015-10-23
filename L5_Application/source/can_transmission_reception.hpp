/*
 * This file includes the function that are needed to send the data over can
 * and receive data from CAN depending on particular filters
 *
 * Author : Gaurao Chaudhari
 * Date   : 10/18/2015
 */

#ifndef CAN_TRANSMISSION_RECEPTION_HPP__
#define CAN_TRANSMISSION_RECEPTION_HPP__

#include "stdio.h"
#include "stdlib.h"
#include "can.h"
#include "io.hpp"

void BusOffCb(uint32_t param);

void DataOverCanBuffer(uint32_t param);

void CANInitialization();

void CANTransmission(can_msg_t canMessageBlock);

void CANReception(can_msg_t canMessageBlock);

#endif /* CAN_TRANSMISSION_RECEPTION_HPP__ */
