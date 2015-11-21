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
#include "source\can_communication_ids.h"
#include "source\can_transmission_reception.h"
#include "file_logger.h"


extern I2C2& i2c;
void compassInitialisation()
{
    void logger_init();
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

     LD.setNumber(heading);
     LOG_INFO( "Heading: %d",heading);
	 return heading;
}

float compassCalibration(float headingNotCalibrated)
{
	float headingCalibrated;
	headingCalibrated = headingNotCalibrated + 0.050536;


	if((headingCalibrated >= -3)&&(headingCalibrated <-2))
	{
		headingCalibrated = (headingCalibrated * 1.35)+ M_TWOPI;
	}
	else if((headingCalibrated >= -2)&&(headingCalibrated <-1))
	{
	    headingCalibrated = (headingCalibrated * 1.4);
	}
	else if((headingCalibrated >= -1)&&(headingCalibrated < 0))
	{
	        headingCalibrated = (headingCalibrated * 1.35);
	}
	else if((headingCalibrated >= 0)&&(headingCalibrated <=1))
	{
		return headingCalibrated;
	}
	else if((headingCalibrated > 1)&&(headingCalibrated <= 2))
	{
		headingCalibrated = headingCalibrated * 0.9;
	}
	else if((headingCalibrated > 2)&&(headingCalibrated <= 3))
	{
	    headingCalibrated = headingCalibrated * 0.78;
	}
	return headingCalibrated;
}

void masterTurnAngle(uint16_t sourceAngle, uint16_t destinationAngle)
{
	/*"degree" is the angle to turn the car
	  if "direction" is 1, car needs to turn anti clockwise
	  if "direction" is 0, car needs to turn clockwise*/
	uint16_t angle = 0;
	if(destinationAngle > sourceAngle)
	{
		angle = destinationAngle - sourceAngle;
		if(angle <= 180)
		{
			/*Turn clockwise as the angle is less in clockwise direction*/
			turn.direction = 2;
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
			turn.direction = 2;
		}
	}
	else if(sourceAngle == destinationAngle)
	{
		angle = 0;
	}

	turn.degree = angle;
    LOG_INFO("Turn Angle: %d, Direction: %d", turn.degree,turn.direction);
	if(!CANTransmit(TTurnAngleToMaster,(uint8_t*)&turn,sizeof(turn)))
	{
	    LE.toggle(3);
	}
}
