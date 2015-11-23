/*
 * compass.hpp
 *
 *  Created on: Oct 23, 2015
 *      Author: Sindhuja
 */

#ifndef L5_APPLICATION_COMPASS_HPP_
#define L5_APPLICATION_COMPASS_HPP_

#include "scheduler_task.hpp"
#include "i2c2.hpp"

//I2C2& i2c = I2C2::getInstance();

void compassInitialisation();
void compassWriteReg();
bool compassReadReg();
int compassHeading();
float compassCalibration(float headingNotCalibrated);
void masterTurnAngle(uint16_t sourceAngle, uint16_t destinationAngle);

#endif /* L5_APPLICATION_COMPASS_HPP_ */
