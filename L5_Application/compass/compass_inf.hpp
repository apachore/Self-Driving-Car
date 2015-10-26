/*
 * compass_inf.hpp
 *
 *  Created on: Oct 23, 2015
 *      Author: Sindhuja
 */

#ifndef L5_APPLICATION_COMPASS_INF_HPP_
#define L5_APPLICATION_COMPASS_INF_HPP_

#include "math.h"


#define STACKMEMORYSIZE 5000
#define I2CBAUDRATE 100
#define DEVICEWRITEADDRESS 0x3C
#define DEVICEREADADDRESS  0x3D
#define HALFCIRCLE 180
#define RADIANTODEGREE  HALFCIRCLE/M_PI

enum compass_RegAddress
{
    ConfigRegA = 0x00,
    ConfigRegB = 0x01,
    ModeReg    = 0x02,
    MSBReg_x   = 0x03,
    LSBReg_x   = 0x04,
    MSBReg_y   = 0x07,
    LSBReg_y   = 0x08,
};

enum register_ConfigValues
{
    /* These values are tentative. The values are taken as default  settings
     specified by the HMC8335L and is subject to change as the project demands */
    ConfigRegA_value = 0x10,
    ConfigRegB_value = 0x00,
    ModeReg_value    = 0x00,
};


const float MagneticDeclination = 0.2361;

#endif /* L5_APPLICATION_COMPASS_INF_HPP_ */
