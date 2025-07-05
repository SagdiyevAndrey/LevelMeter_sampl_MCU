/*
 * mainCycle.h
 *
 *  Created on: May 7, 2025
 *      Author: Andrey Sagdiyev
 */

#ifndef INC_MAINCYCLE_H_
#define INC_MAINCYCLE_H_

#include "stm32u5xx_hal.h"
#include "refCtrl.h"
#include "commands.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

void main_init();
void main_cycle();


#endif /* INC_MAINCYCLE_H_ */
