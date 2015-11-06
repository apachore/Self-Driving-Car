/*
 * compass.cpp
 *
 *  Created on: Oct 23, 2015
 *      Author: Sindhuja
 */

#include "compass.hpp"
#include "compass_inf.hpp"
#include "math.h"
#include "stdio.h"
#include "inttypes.h"
#include "io.hpp"


extern I2C2& i2c;
void compassInitialisation()
{
   /*Check if the i2c slave device is ready for operations*/
	/*if the device is not ready, the onboard LED is switched on*/
	if((i2c.checkDeviceResponse(DEVICEWRITEADDRESS)) == false)
	{
		LE.toggle(1);
	}
	else
	{
		i2c.init(I2CBAUDRATE);
		compassWriteReg();
	}
}

void compassWriteReg()
{
	/*Setting Configuration Register A for
	 number of samples averaged(8 samples),
	 data output rate(75Hz default) ,
	 measurement configuration(Normal measurement config)*/
	i2c.writeReg(DEVICEWRITEADDRESS,ConfigRegA,ConfigRegA_value);

	/*Setting Configuration Register B for device gain
	Gain is set to 1370 LSB/Gauss */
	i2c.writeReg(DEVICEWRITEADDRESS,ConfigRegB,ConfigRegB_value);

	/*Setting Mode register to continuous measurement mode*/
	i2c.writeReg(DEVICEWRITEADDRESS,ModeReg,ModeReg_value);
}

bool compassReadReg()
{
	 /*considering the x and y axis values of the magnetometer for heading calculation
	  y corresponds to the front and back movement; x corresponds to the
	  360 degree rotation in the horizontal plane*/
	raw.x_axis = i2c.readReg(DEVICEREADADDRESS, MSBReg_x)<< 8;
	raw.x_axis = raw.x_axis | (i2c.readReg(DEVICEREADADDRESS, LSBReg_x));
	raw.z_axis = i2c.readReg(DEVICEREADADDRESS, MSBReg_z)<< 8;
	raw.z_axis = raw.z_axis | (i2c.readReg(DEVICEREADADDRESS, LSBReg_z));
	raw.y_axis = i2c.readReg(DEVICEREADADDRESS, MSBReg_y)<< 8;
	raw.y_axis = raw.y_axis | (i2c.readReg(DEVICEREADADDRESS, LSBReg_y));

   /*The raw values are modified to include the appropriate gain settings of the device*/
	/*Resolution is inverse of the gain*/
	scaled.x_axis = raw.x_axis * resolution;
	scaled.y_axis = raw.y_axis * resolution;
	return true;
}

int compassHeading()
{

	 bool registerRead;
	 registerRead = compassReadReg();
     if(registerRead)
     {
    	 puts("Read successful\n");
     }

     float headingRadians;
	 headingRadians = atan2f(scaled.y_axis,scaled.x_axis);
	// printf("Heading: %f\n",headingRadians);

	 /*Calibration will be done before converting to radians to degrees*/

     compassCalibration();

	 /*Compass points to magnetic north. Declination is Angle between true north and magnetic north
	 Magnetic declination at San Francisco is 13.53  degrees east*/
	 headingRadians = headingRadians + MagneticDeclination;


	 if(headingRadians < 0)
	 {
	     headingRadians = headingRadians + M_TWOPI;
	 }
	 if(headingRadians > M_TWOPI)
	 {
	     headingRadians = headingRadians - M_TWOPI;
	 }

	float headingDegrees;
	headingDegrees = headingRadians *  RADIANTODEGREE;
	//printf("Heading: %f\n",headingDegrees);

    int heading;
    heading = static_cast<int>(headingDegrees);
   // printf("Heading in int: %i\n",heading);
	return heading;
}

void compassCalibration()
{
	/*Code for calibration will be included*/
}

int masterTurnAngle(int sourceAngle, int destinationAngle)
{
	/*Bit 8 indicates clockwise or anti clockwise direction
	  if Bit 8 is 1,  car needs to turn anticlockwise
	  if Bit 8 is 0, car needs to turn clockwise*/
	int angle = 0;
	if(destinationAngle > sourceAngle)
	{
		angle = destinationAngle - sourceAngle;
		if(angle <= 180)
		{
			puts("Turn clockwise");
			angle &= ~(1<<8);
		}
		else
		{
			puts("Turn anticlockwise");
			angle = 360 - angle;
			angle |= (1<<8);
		}
	}

	else if(sourceAngle > destinationAngle)
	{
		angle = sourceAngle - destinationAngle;
		if(angle<= 180)
		{
			puts("Turn anticlockwise");
			angle |= (1<<8);
		}
		else
		{
			puts("Turn clockwise");
			angle = 360 - angle;
			angle &= ~(1<<8);
		}
	}
	else if(sourceAngle == destinationAngle)
	{
		angle = 0;
	}
	//printf("angle: %i\n",angle);
	return angle;
}
