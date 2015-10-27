/*
 * Code for base Can communication for receiving the messages.
 * This file includes the class which will define the class needed for implementing CAN
 * reception.
 * Authored : Gaurao Chaudhari
 * Date     : 10/18/2015
 */

#ifndef CAN_BASE_COMMUNICATION_HPP__
#define CAN_BASE_COMMUNICATION_HPP__

#include "scheduler_task.hpp"


/*CAN Task receiver class.
This task will receive all the CAN messages from the CAN bus.*/
class canReceiver : public scheduler_task
{
    public:
        canReceiver(uint8_t priority);
        bool run(void *p);
        bool init(void);

    private:

};


#endif /* CAN_BASE_COMMUNICATION_HPP__ */
