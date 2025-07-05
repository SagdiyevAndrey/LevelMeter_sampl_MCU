#include "cycleSync.h"

#define RESET_PULSE_DELAY_MS			1
#define SET_REG_DELAY_MS				1
#define REG_SETTINGS_LAST_BIT			MASK_BIT_7

void sync_enableSync()
{
	HAL_GPIO_WritePin(SYNC_CLEAR_PORT, SYNC_CLEAR_PIN, GPIO_PIN_RESET);
}
void sync_disableSync()
{
	HAL_GPIO_WritePin(SYNC_CLEAR_PORT, SYNC_CLEAR_PIN, GPIO_PIN_SET);
}
void sync_start()
{
	sync_disableSync();
	HAL_Delay(RESET_PULSE_DELAY_MS);
	sync_enableSync();
}
void sync_enableGate()
{
	HAL_GPIO_WritePin(SYNC_GATE_PORT, SYNC_GATE_PIN, GPIO_PIN_SET);
}
void sync_disableGate()
{
	HAL_GPIO_WritePin(SYNC_GATE_PORT, SYNC_GATE_PIN, GPIO_PIN_RESET);
}
void sync_settings(byte div)
{
	byte reg = div;
	for (byte i = 0; i < 8; i++) {
		HAL_GPIO_WritePin(SYNC_SET_CLK_PORT, SYNC_SET_CLK_PIN, GPIO_PIN_RESET);
		HAL_Delay(SET_REG_DELAY_MS);
		if(REG_SETTINGS_LAST_BIT & reg) {
			HAL_GPIO_WritePin(SYNC_SET_DATA_PORT, SYNC_SET_DATA_PIN, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(SYNC_SET_DATA_PORT, SYNC_SET_DATA_PIN, GPIO_PIN_RESET);
		}
		HAL_Delay(SET_REG_DELAY_MS);
		HAL_GPIO_WritePin(SYNC_SET_CLK_PORT, SYNC_SET_CLK_PIN, GPIO_PIN_SET);
		HAL_Delay(SET_REG_DELAY_MS);
		reg <<= 1;
	}
}
_bool sync_isCycle()
{
	return (_bool)HAL_GPIO_ReadPin(SYNC_CYCLE_PORT, SYNC_CYCLE_PIN);
}
_bool sync_isEnd()
{
	return (_bool)HAL_GPIO_ReadPin(SYNC_END_PORT, SYNC_END_PIN);
}


