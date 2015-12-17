/*========================================
 ==========================================================================================
 Name        : can_transmission_reception.h
 Author      : Bhupendra Naphade
 Version     : 1.0
 Description : Header file for CAN bus communication
 +=============================================================================================
 */
#ifndef L5_APPLICATION_CAN_TRANSMISSION_RECEPTION_H_
#define L5_APPLICATION_CAN_TRANSMISSION_RECEPTION_H_

#include "stdio.h"
#include "stdlib.h"
#include "can.h"
#include "can_communication_ids.h"

void can_Tx_Rx_init();
//void can_Boot_stat();
//void can_Heart_beat();
//
//void can_Tx_Sensor_data();

bool CANTransmit(uint32_t , uint8_t * , uint32_t);
void CANReception(/*can_msg_t canMessageBlock*/);




#endif /* L5_APPLICATION_CAN_TRANSMISSION_RECEPTION_H_ */
