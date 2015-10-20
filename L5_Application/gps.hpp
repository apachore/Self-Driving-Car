/*
 * gps.hpp
 *
 *  Created on: 18-Oct-2015
 *      Author: Amit
 */

#ifndef L5_APPLICATION_GPS_HPP_
#define L5_APPLICATION_GPS_HPP_

#include "scheduler_task.hpp"

class gpsTask : public scheduler_task
{
    private:
        char GPS_Data_Receive[100]={0};
        char *Parsed_GPS_String[13];
    public:
        gpsTask(uint8_t priority);
        void ParseString();
        float calculateBearing();
        float calculateDistance();
        bool run(void *p);
};


#endif /* L5_APPLICATION_GPS_HPP_ */
