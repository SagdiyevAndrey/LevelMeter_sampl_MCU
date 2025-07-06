#include "refCtrl.h"

#define TO_UINT32_PTR(ptr, offset)			(uint32_t*)(&ptr[offset])

#define FLAG_START_DMA					0x01
#define FLAG_STOP_ADC					0x02
#define FLAG_MEAS_CMPL					0x04
#define FLAG_MEAS_START					0x08
#define FLAG_MEAS_END					0x10

uint8_t flags = 0;

ADC_HandleTypeDef* ref_hadc = NULL;
uint16_t ref_adcData[REF_ADC_DATA_LEN];
uint32_t ref_adcDataSize = 0;

uint32_t DMA_nextTrsvLen = 0;

uint32_t ref_testVal = 0;

dByte synt_INT = 0;
dByte synt_FRAC = 0;
dByte synt_MOD = 0;
dByte synt_R_count = 0;
byte synt_refDbl = 0;
byte synt_R_div2 = 0;
byte synt_RF_div = 0;

byte sync_div = 0;

/*void ADCData_toVolts()
{
	float coef = REF_ADC_REF_SUM / (float)(1 << (ref_ADC_bitRate() - 1));
	for (uint32_t i = ref_adcDataSize - 1; i >= 0; i--) {
		float volt = (float)ref_adcData[i] * coef;
		ref_adcData[i] = *((uint16_t*)(&volt));
	}
}*/
HAL_StatusTypeDef HAL_ADC_startDMAChannel(ADC_HandleTypeDef* hadc, const uint32_t* pData, uint32_t Length)
{
	uint32_t LengthInBytes;
	DMA_NodeConfTypeDef node_conf;
	HAL_StatusTypeDef tmp_hal_status;
	/* Check linked list mode */
	if ((hadc->DMA_Handle->Mode & DMA_LINKEDLIST) == DMA_LINKEDLIST) {
	    if ((hadc->DMA_Handle->LinkedListQueue != NULL) && (hadc->DMA_Handle->LinkedListQueue->Head != NULL)) {
	    	/* Length should be converted to number of bytes */
	    	if (HAL_DMAEx_List_GetNodeConfig(&node_conf, hadc->DMA_Handle->LinkedListQueue->Head) != HAL_OK)
	    		return HAL_ERROR;
	    	/* Length should be converted to number of bytes */
	    	if (node_conf.Init.SrcDataWidth == DMA_SRC_DATAWIDTH_WORD) {
	    		/* Word -> Bytes */
	            LengthInBytes = Length * 4U;
	    	} else if (node_conf.Init.SrcDataWidth == DMA_SRC_DATAWIDTH_HALFWORD) {
	            /* Halfword -> Bytes */
	            LengthInBytes = Length * 2U;
	        } else { /* Bytes */
	            /* Same size already expressed in Bytes */
	            LengthInBytes = Length;
	        }
	    	hadc->DMA_Handle->LinkedListQueue->Head->LinkRegisters[NODE_CBR1_DEFAULT_OFFSET] = (uint32_t)LengthInBytes;
	    	hadc->DMA_Handle->LinkedListQueue->Head->LinkRegisters[NODE_CSAR_DEFAULT_OFFSET] = (uint32_t)&hadc->Instance->DR;
	    	hadc->DMA_Handle->LinkedListQueue->Head->LinkRegisters[NODE_CDAR_DEFAULT_OFFSET] = (uint32_t)pData;
	        tmp_hal_status = HAL_DMAEx_List_Start_IT(hadc->DMA_Handle);
	    } else {
	        tmp_hal_status = HAL_ERROR;
	    }
	} else {
	    /* Length should be converted to number of bytes */
	    if (hadc->DMA_Handle->Init.SrcDataWidth == DMA_SRC_DATAWIDTH_WORD) {
	    	/* Word -> Bytes */
	        LengthInBytes = Length * 4U;
	    } else if (hadc->DMA_Handle->Init.SrcDataWidth == DMA_SRC_DATAWIDTH_HALFWORD) {
	        /* Halfword -> Bytes */
	        LengthInBytes = Length * 2U;
	    } else { /* Bytes */
	        /* Same size already expressed in Bytes */
	        LengthInBytes = Length;
	    }
	    tmp_hal_status = HAL_DMA_Start_IT(hadc->DMA_Handle, (uint32_t)&hadc->Instance->DR, (uint32_t)pData, LengthInBytes);
	}
	return tmp_hal_status;
}
void ADC_startDMA(ADC_HandleTypeDef* hadc, const uint32_t* pData, uint32_t Length)
{
	uint32_t LengthInBytes;
	if (hadc->DMA_Handle->Init.SrcDataWidth == DMA_SRC_DATAWIDTH_WORD) {
		/* Word -> Bytes */
		LengthInBytes = Length * 4U;
	} else if (hadc->DMA_Handle->Init.SrcDataWidth == DMA_SRC_DATAWIDTH_HALFWORD) {
		/* Halfword -> Bytes */
		LengthInBytes = Length * 2U;
	} else { /* Bytes */
		/* Same size already expressed in Bytes */
		LengthInBytes = Length;
	}

	/* Configure the source address, destination address, the data size and clear flags */
	MODIFY_REG(hadc->DMA_Handle->Instance->CBR1, DMA_CBR1_BNDT, (LengthInBytes & DMA_CBR1_BNDT));
	/* Clear all interrupt flags */
	__HAL_DMA_CLEAR_FLAG(hadc->DMA_Handle, DMA_FLAG_TC | DMA_FLAG_HT | DMA_FLAG_DTE | DMA_FLAG_ULE | DMA_FLAG_USE | DMA_FLAG_SUSP | DMA_FLAG_TO);
	/* Configure DMA channel source address */
	hadc->DMA_Handle->Instance->CSAR = (uint32_t)&hadc->Instance->DR;
	/* Configure DMA channel destination address */
	hadc->DMA_Handle->Instance->CDAR = (uint32_t)pData;

    /* Enable common interrupts: Transfer Complete and Transfer Errors ITs */
    __HAL_DMA_ENABLE_IT(hadc->DMA_Handle, (DMA_IT_TC | DMA_IT_DTE | DMA_IT_ULE | DMA_IT_USE | DMA_IT_TO));
    __HAL_DMA_ENABLE_IT(hadc->DMA_Handle, DMA_IT_HT);
    /* Enable DMA channel */
    __HAL_DMA_ENABLE(hadc->DMA_Handle);

    MODIFY_REG(ref_hadc->Instance->CR, ADC_CR_BITS_PROPERTY_RS, ADC_CR_ADSTART);

    hadc->DMA_Handle->State = HAL_DMA_STATE_BUSY;
        /* Update the DMA channel error code */
    hadc->DMA_Handle->ErrorCode = HAL_DMA_ERROR_NONE;
}
void ADC_measCpltCallback()
{
	sync_disableGate();
	SET_BIT(flags, FLAG_MEAS_CMPL);
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
}
void HAL_DMA_TrsvCpltCallback(DMA_HandleTypeDef* hdma)
{
	if (ref_hadc->DMA_Handle->Instance != hdma->Instance)
		return;
	if (DMA_nextTrsvLen > 0) {
		SET_BIT(flags, FLAG_START_DMA);
	} else {
		SET_BIT(flags, FLAG_STOP_ADC);
	}
}

void meas_start_DMA(uint32_t dest_ptr_offset, uint32_t size)
{
	ref_hadc->DMA_Handle->XferCpltCallback = HAL_DMA_TrsvCpltCallback;
	ADC_startDMA(ref_hadc, TO_UINT32_PTR(ref_adcData, dest_ptr_offset), size);
}
void meas_start_ADC_DMA(uint32_t size)
{
	ref_hadc->DMA_Handle->XferCpltCallback = HAL_DMA_TrsvCpltCallback;
	if(HAL_ADC_startDMAChannel(ref_hadc, TO_UINT32_PTR(ref_adcData, 0), size) != HAL_OK)
		return;
	if(HAL_ADC_Start_IT(ref_hadc) != HAL_OK)
		HAL_DMA_Abort(ref_hadc->DMA_Handle);

}
void ref_test_set1()
{
	HAL_GPIO_WritePin(REF_TEST_PORT, REF_TEST_PIN, GPIO_PIN_SET);
}
void ref_test_set0()
{
	HAL_GPIO_WritePin(REF_TEST_PORT, REF_TEST_PIN, GPIO_PIN_RESET);
}
void ref_test_send(uint32_t value)
{
	ref_test_set1();
	HAL_Delay(10);
	ref_test_set0();
	HAL_Delay(1);
	for (byte i = 0; i < 32; i++) {
		if (value & 1) {
			ref_test_set1();
		} else {
			ref_test_set0();
		}
		HAL_Delay(1);
		ref_test_set0();
		HAL_Delay(1);
		value >>= 1;
	}
	ref_test_set1();
	HAL_Delay(10);
}
void ref_test_measNum()
{
	ref_testVal = 0;
	for (uint32_t i = 0; i < REF_ADC_DATA_LEN; i++) {
		if (ref_adcData[i] != 0) {
			ref_testVal++;
		} else {

		}
	}
}

void ref_meas_ADC(uint32_t measSize) {
	ref_adcDataSize = 0;
	if (measSize == 0)
		return;
	DMA_nextTrsvLen = 0;
	if (REF_DMA_TRSV_LEN_MAX < measSize) {
		DMA_nextTrsvLen = measSize - REF_DMA_TRSV_LEN_MAX;
		meas_start_ADC_DMA(REF_DMA_TRSV_LEN_MAX);
		ref_adcDataSize = REF_DMA_TRSV_LEN_MAX;
	} else {
		ref_adcDataSize = measSize;
		meas_start_ADC_DMA(measSize);
	}
}
void ref_IT_meas_start()
{
	SET_BIT(flags, FLAG_MEAS_START);
}
void ref_IT_meas_end()
{
	SET_BIT(flags, FLAG_MEAS_END);
}
void ref_sync_settings(byte div)
{
	sync_div = div;
	sync_settings(div);
}
void ref_synt_settings(dByte INT, dByte FRAC, dByte MOD, dByte R_count, byte refDbl, byte R_DIV2, byte RF_div)
{
	synt_INT = INT;
	synt_FRAC = FRAC;
	synt_MOD = MOD;
	synt_R_count = R_count;
	synt_refDbl = refDbl;
	synt_R_div2 = R_DIV2;
	synt_RF_div = RF_div;
	ADF4351_settings(INT, FRAC, MOD, R_count, refDbl, R_DIV2, RF_div);
}
void ref_init(ADC_HandleTypeDef* hadc)
{
	ADF4351_init();
	ref_synt_settings(REF_SYNT_INT, REF_SYNT_FRAC, REF_SYNT_MOD, REF_SYNT_R_COUNT,
			          REF_SYNT_REF_DBL, REF_SYNT_R_DIV2, REF_SYNT_RF_DIV);
	sync_disableGate();
	ref_sync_settings(REF_SYNC_DIV);
	ref_hadc = hadc;
}
void ref_cycle()
{
	if (flags & FLAG_START_DMA) {
		CLEAR_BIT(flags, FLAG_START_DMA);
		if (REF_DMA_TRSV_LEN_MAX < DMA_nextTrsvLen) {
			meas_start_DMA(ref_adcDataSize, REF_DMA_TRSV_LEN_MAX);
			DMA_nextTrsvLen -= REF_DMA_TRSV_LEN_MAX;
			ref_adcDataSize += REF_DMA_TRSV_LEN_MAX;
		} else {
			meas_start_DMA(ref_adcDataSize, DMA_nextTrsvLen);
			DMA_nextTrsvLen = 0;
			ref_adcDataSize += DMA_nextTrsvLen;
		}
		ref_testVal++;
	}
	if (flags & FLAG_STOP_ADC) {
		HAL_ADC_Stop_IT(ref_hadc);
		ADC_measCpltCallback();
		CLEAR_BIT(flags, FLAG_STOP_ADC);
	}
	if (flags & FLAG_MEAS_START) {
		sync_enableGate();
		ref_meas_ADC(ref_adcDataSize);
		CLEAR_BIT(flags, FLAG_MEAS_START);
	}
}
void ref_measure_start(uint32_t measSize)
{
	ref_dataClear();
	ref_adcDataSize = measSize;
	sync_start();
}
void ref_dataClear()
{
	memset(ref_adcData, 0, REF_ADC_DATA_LEN * sizeof(ref_adcData[0]));
	ref_adcDataSize = 0;
}
byte ref_ADC_bitRate()
{
	switch (ref_hadc->Init.Resolution) {
	case ADC_RESOLUTION_14B:
		return 14;
	case ADC_RESOLUTION_12B:
		return 12;
	case ADC_RESOLUTION_10B:
		return 10;
	case ADC_RESOLUTION_8B:
		return 8;
	case ADC_RESOLUTION_6B:
		return 6;
	default:
		return 1;
	}
	return 1;
}
dByte ref_divider()
{
	return (dByte)sync_div * REF_SYNC_PREV_DIV;
}
float ref_synt_freq()
{
	return ADF4351_VCO_freq(ADF4351_CLOCKREF * MHZ, synt_R_div2, synt_refDbl, synt_R_count, synt_INT, synt_FRAC, synt_MOD)
			/ (float)ADF4351_RF_div_val(synt_RF_div);
}
float ref_synt_period()
{
	return 1.0f / ref_synt_freq();
}
float ref_df()
{
	return (float)fabs(ref_synt_freq() - ADF4351_CLOCKREF * MHZ);
}
float ref_dt()
{
	return (float)fabs(1.0f / ref_synt_freq() - 1.0f / (ADF4351_CLOCKREF * MHZ));
}
float ref_df_div()
{
	return ref_df() / (float)ref_divider();
}
float ref_dt_div()
{
	return ref_dt() * (float)ref_divider();
}
uint32_t ref_measNum()
{
	return (uint32_t)round(1.0f / (ref_synt_freq() * ref_dt()));
}
uint32_t ref_ADC_dataSize_byte()
{
	return ref_adcDataSize * REF_DMA_TRSV_DATA_WIDTH;
}
void ref_ADC_data_turn_val() {
	if (ref_adcDataSize == 0)
		return;
	for (uint32_t i = 0; i < ref_adcDataSize; i++) {
		uint16_t LB = ref_adcData[i] & 0x00FF;
		ref_adcData[i] >>= 8;
		ref_adcData[i] |= (LB << 8);
	}
}
_bool ref_isMeasCompleted()
{
	if (flags & FLAG_MEAS_CMPL) {
		CLEAR_BIT(flags, FLAG_MEAS_CMPL);
		return TRUE;
	} else {
		return FALSE;
	}
}
uint32_t ref_testValue()
{
	return ref_testVal;
}

