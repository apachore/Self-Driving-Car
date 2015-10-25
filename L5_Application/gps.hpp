/*===========================================================================================
 Name        : gps.hpp
 Author      : Amit Pachore
 Version     : 1.0
 Description : Header file for class declaration of GPS task
=============================================================================================
*/

#ifndef L5_APPLICATION_GPS_HPP_
#define L5_APPLICATION_GPS_HPP_

#include "scheduler_task.hpp"
#include "Uart3.hpp"



typedef struct
{
        float latitude;
        float longitude;
}coordinates;

class gpsTask : public scheduler_task
{

    public:
        //char message_type[10];
        gpsTask(uint8_t priority);
        bool init(void);
        //void parse_gps_string(void);
        float calculateBearing();
        float calculateDistance();
        bool run(void *p);

    private:
        char GPS_Data_Receive[100];
//        char North_South_Hemisphere = 0;
//        char East_West_Hemisphere = 0;
        char *GPS_parsed_data[14];
        coordinates current_gps_data,gps_readings;
        uint8_t satelite_connections=0;
        Uart3 &gps_uart;
        static const int gps_rx_q_size = 100;

        QueueHandle_t gps_data_q;
};


#endif /* L5_APPLICATION_GPS_HPP_ */
