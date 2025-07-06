#include "mainCycle.h"

#define MAIN_IF_DATA_SIZE					2048
#define MAIN_IF_TIMEOUT						1000
#define IF_FLAG_RECD						0x01
#define IF_DELAY							40
#define TEST_STR_LEN						4

const char str_test[TEST_STR_LEN] = "test";

_bool main_TIM6_isSec = FALSE;
uint16_t main_TIM6_ms = 0;
uint8_t IF_flag = 0;

UART_HandleTypeDef* uart_hdl = &huart4;
uint8_t IF_data[MAIN_IF_DATA_SIZE];
uint16_t IF_dataLen = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM6) {
		main_TIM6_ms++;
		if (main_TIM6_ms >= 1) {
			main_TIM6_isSec = TRUE;
			main_TIM6_ms = 0;
		}
	}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == uart_hdl->Instance)
		SET_BIT(IF_flag, IF_FLAG_RECD);
}
/*void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance == spi_hdl->Instance)
		SET_BIT(IF_flag, IF_FLAG_SPI_RECD);
}*/
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == SYNC_CYCLE_PIN) {
		ref_IT_meas_start();
	} else if (GPIO_Pin == SYNC_END_PIN) {
		ref_IT_meas_end();
	}
}

void main_IF_abort()
{
	HAL_UART_Abort_IT(uart_hdl);
}
uint8_t main_IF_tr_pac(uint8_t* data, uint16_t len) {
	HAL_Delay(IF_DELAY);
	if (HAL_UART_Transmit(uart_hdl, data, len, MAIN_IF_TIMEOUT) == HAL_OK)
		return TRUE;
	return FALSE;
	/*HAL_SPI_Transmit(spi_hdl, (uint8_t*)&len, sizeof(len), MAIN_IF_TRANSMIT_TIMEOUT);*/
}
uint8_t main_IF_rec_pac(uint16_t len) {
	if(HAL_UART_Receive(uart_hdl, IF_data, len, MAIN_IF_TIMEOUT) == HAL_OK)
		return TRUE;
	return FALSE;
}
void main_IF_transmit(uint8_t* data, uint32_t len) {
	main_IF_abort();
	if (!main_IF_tr_pac((uint8_t*)&len, sizeof(len)))
		return;
	while (len > MAIN_IF_DATA_SIZE) {
		if(!main_IF_tr_pac(data, MAIN_IF_DATA_SIZE))
			return;
		if(!main_IF_rec_pac(1))
			return;
		data += MAIN_IF_DATA_SIZE;
		len -= MAIN_IF_DATA_SIZE;
	}
	main_IF_tr_pac(data, len);
}
void main_IF_receive_start() {
	CLEAR_BIT(IF_flag, IF_FLAG_RECD);
	IF_dataLen = 1;
	HAL_UART_Receive_IT(uart_hdl, IF_data, IF_dataLen);
	//HAL_SPI_Receive_IT(spi_hdl, spi_data, spi_dataLen);
}
void main_IF_cmd_test() {
	main_IF_transmit((uint8_t*)str_test, TEST_STR_LEN);
}
void main_IF_cmd_test_value() {
	uint32_t val = ref_testValue();
	main_IF_transmit((uint8_t*)&val, sizeof(val));
}
void main_IF_cmd_test_send_32() {
	main_IF_transmit((uint8_t*)ref_adcData, 32);
}
void main_IF_cmd_test_send_1024() {
	main_IF_transmit((uint8_t*)ref_adcData, 1024);
}
void main_IF_cmd_test_send_4096() {
	main_IF_transmit((uint8_t*)ref_adcData, 4096);
}
void main_IF_cmd_test_send_20000() {
	main_IF_transmit((uint8_t*)ref_adcData, 20000);
}
void main_IF_cmd_test_send_50000() {
	main_IF_transmit((uint8_t*)ref_adcData, 50000);
}
void main_IF_ref_send() {
	uint32_t len = ref_ADC_dataSize_byte();
	main_IF_transmit((uint8_t*)ref_adcData, len);
}

void main_init()
{
	ref_init(&hadc1);
	main_IF_receive_start();
}
void main_cmdProc(byte* data, uint16_t len)
{
	if (len >= 1) {
		switch (data[0]) {
		case CMD_TEST:
			main_IF_cmd_test();
			break;
		case CMD_TEST_VALUE:
			main_IF_cmd_test_value();
			break;
		case CMD_TEST_SEND_32:
			main_IF_cmd_test_send_32();
			break;
		case CMD_TEST_SEND_1024:
			main_IF_cmd_test_send_1024();
			break;
		case CMD_TEST_SEND_4096:
			main_IF_cmd_test_send_4096();
			break;
		case CMD_TEST_SEND_20000:
			main_IF_cmd_test_send_20000();
			break;
		case CMD_TEST_SEND_50000:
			main_IF_cmd_test_send_50000();
			break;
		case CMD_MEAS_START:
			ref_measure_start(1000);
			break;
		default:
			break;
		}
	}
}
void main_IF_cycle()
{
	if (IF_flag & IF_FLAG_RECD) {
		main_cmdProc(IF_data, IF_dataLen);
		main_IF_receive_start();
	}
}
void main_cycle()
{
	ref_cycle();
	main_IF_cycle();
	if (ref_isMeasCompleted()) {
		HAL_Delay(200);
		main_IF_ref_send();
		main_IF_receive_start();
	}
	/*if (ref_isMeasCompleted()) {
		HAL_TIM_Base_Start_IT(&htim6);
	}
    if (main_TIM6_isSec) {
    	ref_adcData[0] = ref_testValue();
    	ref_adcData[1] = 0x0000FFFF;
    	HAL_SPI_Transmit(&hspi3, (byte*)ref_adcData, 8, MAIN_SPI_TIMEOUT_MS);
    	//ref_testValue();
		main_TIM6_isSec = FALSE;
    }*/
}

