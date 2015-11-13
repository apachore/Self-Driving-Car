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
#include "can_communication_ids.h"
#include "can.h"

extern I2C2& i2c;
void compassInitialisation()
{
   /*Check if the i2c slave device is ready for operations*/
	/*if the device is not ready, the onboard LED is switched on*/
	if((i2c.checkDeviceResponse(DEVICEWRITEADDRESS)) == false)
	{
		LE.toggle(1);
	}

	/*Intialise the device and write the compass registers*/
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
	 data output rate(15Hz default) ,
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
     if(!registerRead)
     {
    	 LE.toggle(2);
     }

     float headingRadians, headingRadiansCalibrated;
	 headingRadians = atan2f(scaled.y_axis,scaled.x_axis);

	 /*Compass points to magnetic north. Declination is Angle between true north and magnetic north
	 Magnetic declination at San Francisco is 13.53  degrees east*/
	 headingRadians = headingRadians + MagneticDeclination;
	 printf("Heading in radians: %f\n",headingRadians);

	 /*Calibrate the compass for instrument error*/
	 headingRadiansCalibrated = compassCalibration(headingRadians);

     /*Conversion from degrees to radians*/
	 float headingDegrees;
	 headingDegrees = headingRadiansCalibrated *  RADIANTODEGREE;

     /*Conversion from float to integer*/
     int heading;
     heading = static_cast<int>(headingDegrees);
     printf("Heading in int: %i\n",heading);
	 return heading;
}

float compassCalibration(float headingNotCalibrated)
{
	float headingCalibrated;
	headingCalibrated = headingNotCalibrated + 0.267;
	if(headingCalibrated < 0)
	{
		headingCalibrated = (headingCalibrated * 1.35)+ M_TWOPI;
	}
	else if(headingCalibrated > 0)
	{
		headingCalibrated = headingCalibrated * 0.73;
	}
	return headingCalibrated;
}

void masterTurnAngle(int sourceAngle, int destinationAngle)
{
	/*"degree" is the angle to turn the car
	  if "direction" is 1, car needs to turn anti clockwise
	  if "direction" is 0, car needs to turn clockwise*/
	int angle = 0;
	if(destinationAngle > sourceAngle)
	{
		angle = destinationAngle - sourceAngle;
		if(angle <= 180)
		{
			/*Turn clockwise as the angle is less in clockwise direction*/
			turn.direction = 0;
		}
		else
		{
			/*Turn Anticlockwise as the angle is less in anticlockwise direction*/
			angle = 360 - angle;
			turn.direction = 1;
		}
	}

	else if(sourceAngle > destinationAngle)
	{
		angle = sourceAngle - destinationAngle;
		if(angle<= 180)
		{
			/*Turn Anticlockwise as the angle is less in anticlockwise direction*/
			turn.direction = 1;
		}
		else
		{
			/*Turn clockwise as the angle is less in clockwise direction*/
			angle = 360 - angle;
			turn.direction = 0;
		}
	}
	else if(sourceAngle == destinationAngle)
	{
		angle = 0;
	}

	turn.degree = angle;


	/*Temporary can structure for transmitting turn angle*/
	can_msg_t tx;
	tx.msg_id = TTurnAngle;
	tx.frame_fields.is_29bit = 0;
	tx.frame_fields.data_len = 2;
    tx.data.bytes[0] = turn.degree;
	tx.data.bytes[1] = turn.direction;

	if (!CAN_tx(can1,&tx,0))
	{
	    LE.toggle(3);
	}

}
