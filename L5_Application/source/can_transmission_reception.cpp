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
#include "can_transmission_reception.hpp"

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

void CANInitialization()
{
    CAN_init(can1, 100, 20, 20, *BusOffCb,*DataOverCanBuffer);
    CAN_bypass_filter_accept_all_msgs();


/*  can_std_id_t msg1_std[2];
    msg1_std[0].id = 0x3;
    msg1_std[0].can_num = 0;
    msg1_std[0].disable = 0;
    msg1_std[1].id = 0x4;
    msg1_std[1].can_num = 0;
    msg1_std[1].disable = 0;

    can_std_grp_id_t *grplist=NULL;
    can_ext_id_t *ext=NULL;
    can_ext_grp_id_t *extgrp=NULL;
    CAN_setup_filter(msg1_std,2,grplist,0,ext,0,extgrp,0);*/

}

void CANTransmission(can_msg_t canMessageBlock)
{
    canMessageBlock.msg_id = 0x2;
    canMessageBlock.frame_fields.data_len = 8;
    canMessageBlock.data.qword = 0x222;

    bool flag = CAN_tx(can1,&canMessageBlock, 1000);
}

void CANReception(can_msg_t canMessageBlock)
{
    bool flag = CAN_rx(can1, &canMessageBlock, 1000);
    printf("%x\n", canMessageBlock.msg_id);

    if(canMessageBlock.msg_id == 0x3){
        LE.on(1);
    }
    else if(canMessageBlock.msg_id == 0x4) {
        LE.off(1);
    }
}

#endif /* CAN_TRANSMISSION_RECEPTION_HPP__ */
