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

  compass_task::compass_task():
  scheduler_task("compass_task", STACKMEMORYSIZE, PRIORITY_LOW),
  i2c(I2C2::getInstance())
  {
      i2c.init(I2CBAUDRATE);
      Heading_radians = 0.0;
      Heading_degrees = 0.0;
      resolution = 0.73;

  }

  bool compass_task:: init(void)
  {
      /*Setting Configuration Register A for
        number of samples averaged(11 -> 8 samples),
        data output rate(100 -> 15Hz default) ,
        measurement configuration(00-> Normal measurement config)*/
       i2c.writeReg(DEVICEWRITEADDRESS,ConfigRegA,ConfigRegA_value);

      /*Setting Configuration Register B for device gain
        Gain is set to 390 LSB/Gauss */
      i2c.writeReg(DEVICEWRITEADDRESS,ConfigRegB,ConfigRegB_value);

      /*Setting Mode register to continuous measurement mode*/
      i2c.writeReg(DEVICEWRITEADDRESS,ModeReg,ModeReg_value);
        return true;
  }

  void compass_task::regRead(void)
  {
      /*considering the x and y axis values of the magnetometer for heading calculation
      y corresponds to the front and back movement; x corresponds to the
      360 degree rotation in the horizontal plane*/
      raw.x_axis = i2c.readReg(DEVICEREADADDRESS, MSBReg_x)<< 8;
      raw.x_axis = raw.x_axis | (i2c.readReg(DEVICEREADADDRESS, LSBReg_x));
      raw.y_axis = i2c.readReg(DEVICEREADADDRESS, MSBReg_y)<< 8;
      raw.y_axis = raw.y_axis | (i2c.readReg(DEVICEREADADDRESS, LSBReg_y));
      scaled.x_axis = raw.x_axis * resolution;
      scaled.y_axis = raw.y_axis * resolution;
  }

  bool compass_task::run(void *p)
  {

      regRead();

      Heading_radians = atan2f(scaled.y_axis,scaled.x_axis);
      printf("Heading: %f\n",Heading_radians);

      /*Compass points to magnetic north. Declination is Angle between true north and magnetic north
      Magnetic declination at San Francisco is 13.53  degrees east*/
      Heading_radians = Heading_radians + MagneticDeclination;
      if(Heading_radians < 0)
      {
          Heading_radians = Heading_radians + M_TWOPI;
      }
      if(Heading_radians > M_TWOPI)
      {
          Heading_radians = Heading_radians - M_TWOPI;
      }
      /* Calibrating the compass for instrument error is on progress.
       Will be updated in the subsequent versions of the code*/

      Heading_degrees = Heading_radians *  RADIANTODEGREE;

      printf("x y : %" PRIi16 " %" PRIi16 " ", raw.x_axis, raw.y_axis);
      printf("scaled x y: %f %f\n", scaled.x_axis, scaled.y_axis);
      printf("Heading: %f\n",Heading_degrees);


      vTaskDelay(1000);
      return true;
  }
