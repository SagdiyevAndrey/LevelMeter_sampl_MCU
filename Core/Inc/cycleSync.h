#ifndef CYCLESYNC_H
#define CYCLESYNC_H

#include "comDef.h"

#define SYNC_ADC_CLK_PORT						GPIOC
#define SYNC_ADC_CLK_PIN						GPIO_PIN_15
#define SYNC_END_PORT							GPIOC
#define SYNC_END_PIN							GPIO_PIN_14
#define SYNC_CYCLE_PORT							GPIOB
#define SYNC_CYCLE_PIN							GPIO_PIN_6
#define SYNC_GATE_PORT							GPIOD
#define SYNC_GATE_PIN							GPIO_PIN_7
#define SYNC_CLEAR_PORT							GPIOD
#define SYNC_CLEAR_PIN							GPIO_PIN_6
#define SYNC_SET_CLK_PORT						GPIOD
#define SYNC_SET_CLK_PIN						GPIO_PIN_5
#define SYNC_SET_DATA_PORT						GPIOD
#define SYNC_SET_DATA_PIN						GPIO_PIN_4

void sync_enableSync();
void sync_disableSync();
void sync_start();
void sync_enableGate();
void sync_disableGate();
void sync_settings(byte div);
_bool sync_isCycle();
_bool sync_isEnd();

#endif
