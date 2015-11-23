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


//typedef struct                  //structure to store sensor distance
//{
//        uint64_t front:8;
//        uint64_t right:8;
//        uint64_t left:8;
//        uint64_t back:8;
//}sensor_data_t;
//sensor_data_t sensor_data_obj;


bool sensor_init();
bool sensor_compute();
        //QueueHandle_t sensor_data_q;



#endif /* L5_APPLICATION_SENSOR_HPP_ */
