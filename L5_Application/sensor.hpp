/*===========================================================================================
  Name        : sensors.hpp
  Author      : Bhupendra Naphade
  Version     : 1.0
+ Description : Header file for class declaration of SesnorTask
+=============================================================================================
 */

#ifndef L5_APPLICATION_SENSOR_HPP_
#define L5_APPLICATION_SENSOR_HPP_

#define mydel 50000


        bool sensorTask_init();
        bool sensorTask_run();
        //QueueHandle_t sensor_data_q;



#endif /* L5_APPLICATION_SENSOR_HPP_ */
