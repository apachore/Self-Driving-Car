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

void BusOffCb(uint32_t param)
{
    printf("BusOffCb : Error");
    CAN_reset_bus(can1);
}

void DataOverCanBuffer(uint32_t param)
{
    printf("DataOverCanBuffer : Error");
    CAN_reset_bus(can1);
}

canReceiver::canReceiver(uint8_t priority) : scheduler_task("CAN Base Communication", 500, priority)
{

}

bool canReceiver::init()
{
    CAN_init(can1, 100, 20, 20, *BusOffCb,*DataOverCanBuffer);
    CAN_bypass_filter_accept_all_msgs();

    return true;
}

bool canReceiver::run(void *p)
{
    can_msg_t canMessageBlock;
    bool flag = CAN_rx(can1, &canMessageBlock, 1000);
    return true;
}
