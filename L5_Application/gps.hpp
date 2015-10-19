/*
 * gps.hpp
 *
 *  Created on: 18-Oct-2015
 *      Author: Amit
 */

#ifndef L5_APPLICATION_GPS_HPP_
#define L5_APPLICATION_GPS_HPP_


class gpsTask : public scheduler_task
{
    public:
        gpsTask(uint8_t priority) :
            scheduler_task("gps", 4*512, priority)
        {
            /* Nothing to init */
        }

        bool run(void *p)
        {

            return true;
        }
};


#endif /* L5_APPLICATION_GPS_HPP_ */
