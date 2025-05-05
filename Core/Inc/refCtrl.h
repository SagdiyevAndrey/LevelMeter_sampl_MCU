/*
 * refCtrl.h
 *
 *  Created on: Apr 24, 2025
 *      Author: Andrey Sagdiyev
 */

#ifndef INC_REFCTRL_H_
#define INC_REFCTRL_H_

#include "cycleSync.h"
#include "ADF4351.h"
#include "adc.h"

#define REF_TEST_PORT					GPIOC
#define REF_TEST_PIN					GPIO_PIN_10

#define REF_ADC_REF_P					3.3f
#define REF_ADC_REF_M					0.0f
#define REF_ADC_REF_SUM					REF_ADC_REF_P - REF_ADC_REF_M
#define REF_ADC_DATA_LEN				130000
#define REF_FREQ0_REC					ADF4351_CLOCKREF
#define REF_SYNC_PREV_DIV				2
#define REF_SYNC_DIV					25
#define REF_SYNC_DIV_SUM				REF_SYNC_DIV + REF_SYNC_PREV_DIV
#define REF_DMA_TRSV_LEN_MAX			65535 / 4

extern ADC_HandleTypeDef* ref_hadc;
extern uint32_t ref_adcData[REF_ADC_DATA_LEN];
extern uint32_t ref_adcDataSize;

void ref_init(ADC_HandleTypeDef* hadc);
void ref_cycle();
void ref_measure();
void ref_dataClear();
byte ref_ADC_bitRate();
float ref_df();
float ref_dt();
float ref_df_div();
float ref_dt_div();
void ref_testValue();

#endif /* INC_REFCTRL_H_ */
