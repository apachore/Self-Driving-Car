/*
 * MOTOR_CAN_HEADER.hpp
 *
 *  Created on: 26-Oct-2015
 *      Author: Su
 */

#ifndef L5_APPLICATION_MOTOR_CAN_HEADER_HPP_
#define L5_APPLICATION_MOTOR_CAN_HEADER_HPP_


#include "scheduler_task.hpp"

/*CAN Task receiver class.
This task will receive all the CAN messages from the CAN bus.*/
class canTransmitReceiver : public scheduler_task
{
    public:
        canTransmitReceiver(uint8_t priority);
        bool run(void *p);
        bool init(void);
        void Receiver_message(void);
        void Receive_init();
//        void BusOffCb(uint32_t param);
//        void DataOverCanBuffer(uint32_t param);

    private:
        QueueHandle_t Master_Motor_q;

};


#endif /* L5_APPLICATION_MOTOR_CAN_HEADER_HPP_ */
