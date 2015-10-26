/*===========================================================================================
  Name        : sensors.hpp
  Author      : Bhupendra Naphade
  Version     : 1.0
+ Description : Header file for class declaration of SesnorTask
+=============================================================================================
 */

#ifndef L5_APPLICATION_SENSOR_HPP_
#define L5_APPLICATION_SENSOR_HPP_

class sensorTask : public scheduler_task
{
    public:
        sensorTask(uint8_t priority) ;
                bool init(void);                ///< Inits the task
                bool run(void *p);              ///< The main loop

    private :

                      QueueHandle_t sensor_data_q;
};


#endif /* L5_APPLICATION_SENSOR_HPP_ */
