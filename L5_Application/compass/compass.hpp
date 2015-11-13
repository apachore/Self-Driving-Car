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
void masterTurnAngle(int sourceAngle, int destinationAngle);

/*class compass_task : public scheduler_ask
{
    public:
    compass_task();
    bool init(void);
    void regRead(void);
    bool run(void *p);

    private:
    I2C2& i2c;
    struct compassRawValue
    {
      int16_t x_axis;
      int16_t y_axis;
      int16_t z_axis;
    }raw;

    struct compassGainSettings
    {
        float x_axis;
        float y_axis;
    }scaled;
    float Heading_radians;
    float Heading_degrees;
    float resolution;
};*/



#endif /* L5_APPLICATION_COMPASS_HPP_ */
