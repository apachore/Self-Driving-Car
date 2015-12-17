/*===========================================================================================
  Name        : sensors.hpp
  Author      : Bhupendra Naphade
  Version     : 1.0
+ Description : Header file for class declaration of SesnorTask
+=============================================================================================
 */

#ifndef L5_APPLICATION_SENSOR_HPP_
#define L5_APPLICATION_SENSOR_HPP_
#include <stdint.h>


typedef struct                  //structure to store obstacle distance by sensor
{
        uint8_t front ;
        uint8_t back  ;
        uint8_t right ;
        uint8_t left  ;
}sensor_data_t;

bool sensor_init();
bool sensor_compute();




#endif /* L5_APPLICATION_SENSOR_HPP_ */
