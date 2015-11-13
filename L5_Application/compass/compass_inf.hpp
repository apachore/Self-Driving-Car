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
#define COMPASSADDRESS 0x1E
#define DEVICEWRITEADDRESS 0x3C
#define DEVICEREADADDRESS  0x3D
#define HALFCIRCLE 180
#define RADIANTODEGREE  HALFCIRCLE/M_PI

const float MagneticDeclination = 0.2361;

/*Corresponds to gain of 1370. Resolution is inverse of Gain -> 0.73 milli gauss per count*/
float resolution = 0.73;

 struct compassRawValue
 {
     int16_t x_axis;
     int16_t y_axis;
     int16_t z_axis;
 }raw;

 struct compassScaledValue
 {
      float x_axis;
      float y_axis;
 }scaled;


/* List of register address*/
/*8 bit onchip registers for reading from and writing to HMC5883L compass module*/
enum compassRegAddress
{
    ConfigRegA = 0x00,
    ConfigRegB = 0x01,
    ModeReg    = 0x02,
    MSBReg_x   = 0x03,
    LSBReg_x   = 0x04,
	MSBReg_z   = 0x05,
	LSBReg_z   = 0x06,
    MSBReg_y   = 0x07,
    LSBReg_y   = 0x08,
};

/*Option for number of samples averaged per measurement output for setting Configuration Register A*/
enum compassNoOfSamplesAvg
{
	One = 0x00,
    Two = 0x20,
    Four = 0x40,
    Eight = 0x60,
};

/*Option for selectable output rates in continuous measurement mode for setting Configuration Register A*/
enum compassDataOutputRate
{
	_point75Hz = 0x00,
	_1point5Hz = 0x01,
	_3Hz = 0x02,
	_7point5Hz = 0x03,
	_15Hz = 0x04,
	_30Hz = 0x05,
	_75Hz = 0x06,
	_reserved = 0x07,
};

/*Option for Measurement Mode configuration for setting Configuration Register A*/
enum compassMeasurementMode
{
	Normal = 0x00,
	Positive = 0x01,
	Negative = 0x02,
};

enum compassGainSettings
{
   Gain_1370 = 0x00,
   Gain_1090 = 0x01,
   Gain_820 = 0x02,
   Gain_660 = 0x03,
   Gain_440 = 0x04,
   Gain_390 = 0x05,
   Gain_330 = 0x06,
   Gain_230 = 0x07,
};

/*Option for operating modes of the compass module for setting Mode Register */
enum compassOperatingModes
{
	Continuous = 0x00,
	Single = 0x01,
	Idle = 0x02,
};


enum compassRegConfigValues
{
    /* These values are tentative. The values are taken as default  settings
     specified by the HMC8335L and is subject to change as the project demands */
    ConfigRegA_value = (Eight|_15Hz|Normal),
    ConfigRegB_value = Gain_1370,
    ModeReg_value    = Continuous,
};

struct angle
 {
	 uint8_t degree;
	 uint8_t direction;
 }turn;
#endif /* L5_APPLICATION_COMPASS_INF_HPP_ */
