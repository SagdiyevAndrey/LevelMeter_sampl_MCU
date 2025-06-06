/*
 * mainCycle.h
 *
 *  Created on: May 7, 2025
 *      Author: Andrey Sagdiyev
 */

#ifndef INC_MAINCYCLE_H_
#define INC_MAINCYCLE_H_

#include "refCtrl.h"
#include "tim.h"
#include "spi.h"

#define MAIN_SPI_TIMEOUT_MS				1000

void main_init();
void main_cycle();


#endif /* INC_MAINCYCLE_H_ */
