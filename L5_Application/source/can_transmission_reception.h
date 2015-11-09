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
#include "gps.hpp"

#include "can_communication_ids.h"

//bool BootReplySent; //Used as indication of system startup flag

//class CANTransmissionReception
//{
//    public:
//        CANTransmissionReception(/*can_msg_t canMessageBlock*/);

        /* This function would initialize the CAN bus. It will setup the filter that is to be used for receiving
           messages. */
        void CANInitialization();
        bool CANTransmit(uint32_t , uint8_t * , uint32_t);
        void CANReception(/*can_msg_t canMessageBlock*/);

        // This function would be needed later in development
        void CANSetupFilter();

        void CANMessageProcessing();
        //void CANTransmitBootReply();
        //void CANTransmitCoordinates(coordinates);

//    private:
        //extern can_msg_t canMessageReceivedBlock;
        //extern can_msg_t canMessageTransmittedBlock;
        //extern QueueHandle_t CAN_received_Data_Queue;
        /*bool firstTime;*/
        // Define a Filter List;
//};




#endif /* L5_APPLICATION_SOURCE_CAN_TRANSMISSION_RECEPTION_H_ */
