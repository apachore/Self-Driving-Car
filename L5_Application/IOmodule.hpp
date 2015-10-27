/*
 * IOmodule.hpp
 *
 *  Created on: 26-Oct-2015
 *      Author: User
 */

#ifndef L5_APPLICATION_IOMODULE_HPP_
#define L5_APPLICATION_IOMODULE_HPP_

#include "can.h"
#include "char_dev.hpp"
#include "tasks.hpp"
#include "file_logger.h"

class IOTask : public scheduler_task
{
    public:
        IOTask(uint8_t priority);
        bool init(void);
        bool run(void *p);
        can_msg_t rx_msg, ack_msg1;
    private:
           Uart2 &IO_uart;
           static const int io_rx_q_size=100;
           static const int io_tx_q_size=1;
};
#endif /* L5_APPLICATION_IOMODULE_HPP_ */
