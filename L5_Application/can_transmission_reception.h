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
//#include "compass/compass.hpp"

#include "can_communication_ids.h"


        void CANInitialization();
        bool CANTransmit(uint32_t , uint8_t * , uint32_t);
        //void CANReception(/*can_msg_t canMessageBlock*/);


        void CANSetupFilter();

        //void CANMessageProcessing();




#endif /* L5_APPLICATION_SOURCE_CAN_TRANSMISSION_RECEPTION_H_ */
