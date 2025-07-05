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
#define REF_ADC_DATA_LEN				120000
#define REF_FREQ0_REC					ADF4351_CLOCKREF
#define REF_SYNC_PREV_DIV				2
#define REF_DMA_TRSV_DATA_WIDTH			2
#define REF_DMA_TRSV_LEN_MAX			65535 / REF_DMA_TRSV_DATA_WIDTH

// Default ADF4351 register values
#define REF_SYNT_INT					128
#define REF_SYNT_FRAC					1
#define REF_SYNT_MOD					1000
#define REF_SYNT_R_COUNT				1
#define REF_SYNT_REF_DBL				ADF4351_DBLBUF_DISABLED
#define REF_SYNT_R_DIV2					ADF4351_RDIV2_ENABLED
#define REF_SYNT_RF_DIV					ADF4351_RFDIVSEL_64

// Default dividers values
#define REF_SYNC_DIV					25
#define REF_SYNC_DIV_SUM				REF_SYNC_DIV * REF_SYNC_PREV_DIV

extern uint16_t ref_adcData[REF_ADC_DATA_LEN];
extern uint32_t ref_adcDataSize;


void ref_IT_meas_start();
void ref_IT_meas_end();

void ref_sync_settings(byte div);
void ref_synt_settings(dByte INT, dByte FRAC, dByte MOD, dByte R_count, byte refDbl, byte R_DIV2, byte RF_div);
void ref_init(ADC_HandleTypeDef* hadc);
void ref_cycle();
void ref_measure_start(uint32_t measSize);
void ref_dataClear();
byte ref_ADC_bitRate();
dByte ref_divider();
// Clock frequency at input synthesizer (clock for receiver)
float ref_synt_in_freq(); // [Hz]
// Clock period at input synthesizer
float ref_synt_in_period(); // [s]
// Clock frequency at output synthesizer (clock for transmitter)
float ref_synt_freq(); // [Hz]
// Clock period at output synthesizer
float ref_synt_period(); // [s]
// Frequency difference
float ref_df(); // [Hz]
// Sampling time
float ref_dt(); // [s]
float ref_df_div(); // [Hz]
float ref_dt_div(); // [s]
uint32_t ref_measNum();
uint32_t ref_ADC_dataSize_byte();
void ref_ADC_data_turn_val();
_bool ref_isMeasCompleted();
uint32_t ref_testValue();

#endif /* INC_REFCTRL_H_ */
