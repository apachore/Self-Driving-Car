#ifndef L5_APPLICATION_SOURCE_BLUETOOTH_HPP_
#define L5_APPLICATION_SOURCE_BLUETOOTH_HPP_

#include "scheduler_task.hpp"

class bluetoothTask : public scheduler_task
{

    public:
        //char message_type[10];
        bluetoothTask(uint8_t priority);
        bool init(void);
        bool run(void *p);

};

#endif /* L5_APPLICATION_SOURCE_BLUETOOTH_HPP_ */

