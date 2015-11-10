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
#include "can_comms.hpp"
#include "uart2.hpp"

void UARTInitialization();
void LCDdisplay();

#endif /* L5_APPLICATION_IOMODULE_HPP_ */
