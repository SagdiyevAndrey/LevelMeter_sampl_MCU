/*
 * def_pins.h
 *
 *  Created on: Jul 1, 2025
 *      Author: Andrey
 */

#ifndef INC_DEF_PINS_H_
#define INC_DEF_PINS_H_

// ---------------- ADF4351 definitions ----------------
#define DEF_ADF4351_CLOCKREF				50.0f // MHz
// Multiplexer output (unused)
#define DEF_ADF4351_MUXOUT_PORT				GPIOC
#define DEF_ADF4351_MUXOUT_PIN				GPIO_PIN_1
// Lock detect output pin
#define DEF_ADF4351_LD_PORT					GPIOA
#define DEF_ADF4351_LD_PIN					GPIO_PIN_2
// Serial clock input
#define DEF_ADF4351_CLK_PORT				GPIOA
#define DEF_ADF4351_CLK_PIN					GPIO_PIN_1
// Serial data input
#define DEF_ADF4351_DATA_PORT				GPIOA
#define DEF_ADF4351_DATA_PIN				GPIO_PIN_0
// Load enable (input)
#define DEF_ADF4351_LE_PORT					GPIOC
#define DEF_ADF4351_LE_PIN					GPIO_PIN_3
// Chip enable (input)
#define DEF_ADF4351_CE_PORT					GPIOC
#define DEF_ADF4351_CE_PIN					GPIO_PIN_2

#endif /* INC_DEF_PINS_H_ */
