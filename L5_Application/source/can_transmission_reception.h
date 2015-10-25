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

class CANTransmissionReception
{
    public:
        CANTransmissionReception(/*can_msg_t canMessageBlock*/);

        /* This function would initialize the CAN bus. It will setup the filter that is to be used for receiving
           messages. */
        void CANInitialization();
        void CANTransmission(/*can_msg_t canMessageBlock*/);
        void CANReception(/*can_msg_t canMessageBlock*/);

        // This function would be needed later in development
        void CANSetupFilter();

    private:
        can_msg_t canMessageReceivedBlock;
        can_msg_t canMessageTransmittedBlock;
        QueueHandle_t receivedDataQueue;
        /*bool firstTime;*/
        // Define a Filter List;
};




#endif /* L5_APPLICATION_SOURCE_CAN_TRANSMISSION_RECEPTION_H_ */
