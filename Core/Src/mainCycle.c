#include "mainCycle.h"

_bool main_TIM6_isSec = FALSE;
uint16_t main_TIM6_ms = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM6) {
		main_TIM6_ms++;
		if (main_TIM6_ms >= 1) {
			main_TIM6_isSec = TRUE;
			main_TIM6_ms = 0;
		}
	}
}
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance == hspi3.Instance) {

	}
}
void main_init()
{
	ref_init(&hadc1);
	ref_measure();
}
void main_cmdProc(byte* data)
{

}
void main_cycle()
{
	ref_cycle();
	if (ref_isMeasCompleted()) {
		HAL_TIM_Base_Start_IT(&htim6);
	}
    if (main_TIM6_isSec) {
    	ref_adcData[0] = ref_testValue();
    	ref_adcData[1] = 0x0000FFFF;
    	HAL_SPI_Transmit(&hspi3, (byte*)ref_adcData, 8, MAIN_SPI_TIMEOUT_MS);
    	//ref_testValue();
		main_TIM6_isSec = FALSE;
    }
}

