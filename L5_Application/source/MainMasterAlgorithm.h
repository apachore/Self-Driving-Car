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

class MasterAlgorithm : public scheduler_task
{
    public:
        MasterAlgorithm(uint8_t priority);
        void DecisionAlgorithm(can_msg_t canData);
        bool run(void *p);
        bool init(void);

    private:
        QueueHandle_t receivedDataQueue;
        can_msg_t canReceivedData;
        SensorData receivedSensorData;
        bool motorStraight;
        bool motorReverse;
        bool motorLeft;
        bool motorRight;
};


#endif /* L5_APPLICATION_SOURCE_MAINMASTERALGORITHM_H_ */
