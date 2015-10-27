/*
 * Code for base Can communication for receiving the messages.
 * This file includes the class definition needed for implementing CAN
 * reception.
 * Authored : Gaurao Chaudhari
 * Date     : 10/18/2015
 */

/* BASIC Working of CAN Bus. This will be refactored once the details of CAN messages
 * will be more clear
 */

#include "stdio.h"
#include "string.h"
#include "can.h"
#include "can_base_communication.hpp"
#include "can_transmission_reception.h"

canReceiver::canReceiver(uint8_t priority) : scheduler_task("CAN Base Communication", 500, priority)
{

}

bool canReceiver::run(void *p)
{
    can_msg_t canMessageBlock;


    return true;
}
