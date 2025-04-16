#include "ADF4351.h"

#define MASK_UINT_HIGH_BIT				0x80000000
#define MASK_1BIT						0x00000001
#define MASK_2BIT						0x00000003
#define MASK_3BIT						0x00000007
#define MASK_4BIT						0x0000000F
#define MASK_8BIT						0x000000FF
#define MASK_10BIT						0x000003FF
#define MASK_12BIT						0x00000FFF
#define MASK_16BIT						0x0000FFFF

#define ADF4351_CTRL_BITS_REG0			0
#define ADF4351_CTRL_BITS_REG1			1
#define ADF4351_CTRL_BITS_REG2			2
#define ADF4351_CTRL_BITS_REG3			3
#define ADF4351_CTRL_BITS_REG4			4
#define ADF4351_CTRL_BITS_REG5			5

#define ADF4351_BITPOS_FRACVALUE		3
#define ADF4351_BITPOS_INTVALUE			15
#define ADF4351_BITPOS_MODVALUE			3
#define ADF4351_BITPOS_PHASEVALUE		15
#define ADF4351_BITPOS_PSC				27
#define ADF4351_BITPOS_PHASEADJ			28
#define ADF4351_BITPOS_COUNTRST			3
#define ADF4351_BITPOS_CP3STATE			4
#define ADF4351_BITPOS_PWRDOWN			5
#define ADF4351_BITPOS_PDPOLARITY		6
#define ADF4351_BITPOS_LDP				7
#define ADF4351_BITPOS_LDF				8
#define ADF4351_BITPOS_CHRGPUMPCURSET	9
#define ADF4351_BITPOS_DBLBUF			13
#define ADF4351_BITPOS_RCOUNT			14
#define ADF4351_BITPOS_RDIV2			24
#define ADF4351_BITPOS_REFDBL			25
#define ADF4351_BITPOS_MUXOUT			26
#define ADF4351_BITPOS_LOWNOISEMODE		29
#define ADF4351_BITPOS_CLKDIV			3
#define ADF4351_BITPOS_CLKDIVMODE		15
#define ADF4351_BITPOS_CSR				18
#define ADF4351_BITPOS_CHRGCANCEL		21
#define ADF4351_BITPOS_ABP				22
#define ADF4351_BITPOS_BANDSELCLKMODE	23
#define ADF4351_BITPOS_OUTPWR			3
#define ADF4351_BITPOS_RFOUT			5
#define ADF4351_BITPOS_AUXOUTPWR		6
#define ADF4351_BITPOS_AUXOUT			8
#define ADF4351_BITPOS_AUXOUTSEL		9
#define ADF4351_BITPOS_MTLD				10
#define ADF4351_BITPOS_VCOPWRDOWN		11
#define ADF4351_BITPOS_BANDSELCLKDIV	12
#define ADF4351_BITPOS_RFDIVSEL			20
#define ADF4351_BITPOS_FDBCKSEL			23
#define ADF4351_BITPOS_REG5_RESERVED	19
#define ADF4351_BITPOS_LDPINMODE		22

#define SEND_DATA_DELAY_MS				10
#define SETTINGS_REG_DELAY_MS			50

void ADF4351_setDATA(byte level)
{
	HAL_GPIO_WritePin(ADF4351_DATA_PORT, ADF4351_DATA_PIN, level);
}
void ADF4351_setCLK(byte level)
{

	HAL_GPIO_WritePin(ADF4351_CLK_PORT, ADF4351_CLK_PIN, level);
}
void ADF4351_setLE(byte level)
{
	HAL_GPIO_WritePin(ADF4351_LE_PORT, ADF4351_LE_PIN, level);
}
void ADF4351_addSubReg(uint* reg, uint subReg, byte pos)
{
	subReg <<= pos;
	*reg |= subReg;
}
void ADF4351_initPorts()
{
	GPIO_InitTypeDef GPIO_initStruct = {0};

	GPIO_initStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_initStruct.Pull = GPIO_NOPULL;

	GPIO_initStruct.Pin = ADF4351_LD_PIN;
	GPIO_initStruct.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(ADF4351_LD_PORT, &GPIO_initStruct);

	GPIO_initStruct.Pin = ADF4351_CLK_PIN;
	GPIO_initStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(ADF4351_CLK_PORT, &GPIO_initStruct);

	GPIO_initStruct.Pin = ADF4351_DATA_PIN;
	GPIO_initStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(ADF4351_DATA_PORT, &GPIO_initStruct);

	GPIO_initStruct.Pin = ADF4351_LE_PIN;
	GPIO_initStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(ADF4351_LE_PORT, &GPIO_initStruct);

	GPIO_initStruct.Pin = ADF4351_CE_PIN;
	GPIO_initStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(ADF4351_CE_PORT, &GPIO_initStruct);

	HAL_GPIO_WritePin(ADF4351_CLK_PORT, ADF4351_CLK_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ADF4351_DATA_PORT, ADF4351_DATA_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ADF4351_LE_PORT, ADF4351_LE_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ADF4351_CE_PORT, ADF4351_CE_PIN, GPIO_PIN_RESET);
}
void ADF4351_init()
{
	ADF4351_initPorts();
	HAL_Delay(20);
	ADF4351_enable();
	HAL_Delay(20);
}
void ADF4351_sendData(uint data)
{
	ADF4351_setCLK(PULSE_LOW);
	__asm__("NOP");
	for (byte i = 0; i < 32; i++) {
		if (data & MASK_UINT_HIGH_BIT) {
			ADF4351_setDATA(PULSE_HIGH);
		} else {
			ADF4351_setDATA(PULSE_LOW);
		}
		__asm__("NOP");
		ADF4351_setCLK(PULSE_HIGH);
		data <<= 1;
		__asm__("NOP");
		ADF4351_setCLK(PULSE_LOW);
	}
	__asm__("NOP");
	ADF4351_setLE(PULSE_HIGH);
	__asm__("NOP");
	__asm__("NOP");
	ADF4351_setLE(PULSE_LOW);
}
byte ADF4351_calcBandSelClkDivMax(float REF_in, dByte R_count, byte refDbl, byte R_DIV2)
{
	return REF_in * (float)(1 + refDbl) / (ADF4351_VCO_BANDSEL_FREQ_MAX * (float)R_count * (float)(1 + R_DIV2));

}
void ADF4351_setRegister0(dByte fracValue, dByte intValue)
{
	intValue &= MASK_16BIT;
	fracValue &= MASK_12BIT;
	uint data = ADF4351_CTRL_BITS_REG0;
	ADF4351_addSubReg(&data, (uint)fracValue, ADF4351_BITPOS_FRACVALUE);
	ADF4351_addSubReg(&data, (uint)intValue, ADF4351_BITPOS_INTVALUE);
	ADF4351_sendData(data);
}
void ADF4351_setRegister1(dByte modValue, dByte phaseValue, byte psc, _bool phaseAdj)
{
	modValue &= MASK_12BIT;
	phaseValue &= MASK_12BIT;
	psc &= MASK_1BIT;
	phaseAdj &= MASK_1BIT;
	uint data = ADF4351_CTRL_BITS_REG1;
	ADF4351_addSubReg(&data, (uint)modValue, ADF4351_BITPOS_MODVALUE);
	ADF4351_addSubReg(&data, (uint)phaseValue, ADF4351_BITPOS_PHASEVALUE);
	ADF4351_addSubReg(&data, (uint)psc, ADF4351_BITPOS_PSC);
	ADF4351_addSubReg(&data, (uint)phaseAdj, ADF4351_BITPOS_PHASEADJ);
	ADF4351_sendData(data);
}
void ADF4351_setRegister2(_bool countRst, _bool CP3state, _bool pwrDown, byte PD_polarity, byte LDP, byte LDF, byte chrgPumpCurSet,
		byte dblBuf, dByte R_count, _bool R_div2, _bool refDbl, byte muxOut, byte lowNoiseMode)
{
	countRst &= MASK_1BIT;
	CP3state &= MASK_1BIT;
	pwrDown &= MASK_1BIT;
	PD_polarity &= MASK_1BIT;
	LDP &= MASK_1BIT;
	LDF &= MASK_1BIT;
	chrgPumpCurSet &= MASK_4BIT;
	dblBuf &= MASK_1BIT;
	R_count &= MASK_10BIT;
	R_div2 &= MASK_1BIT;
	refDbl &= MASK_1BIT;
	muxOut &= MASK_3BIT;
	lowNoiseMode &= MASK_2BIT;
	uint data = ADF4351_CTRL_BITS_REG2;
	ADF4351_addSubReg(&data, (uint)countRst, ADF4351_BITPOS_COUNTRST);
	ADF4351_addSubReg(&data, (uint)CP3state, ADF4351_BITPOS_CP3STATE);
	ADF4351_addSubReg(&data, (uint)pwrDown, ADF4351_BITPOS_PWRDOWN);
	ADF4351_addSubReg(&data, (uint)PD_polarity, ADF4351_BITPOS_PDPOLARITY);
	ADF4351_addSubReg(&data, (uint)LDP, ADF4351_BITPOS_LDP);
	ADF4351_addSubReg(&data, (uint)LDF, ADF4351_BITPOS_LDF);
	ADF4351_addSubReg(&data, (uint)chrgPumpCurSet, ADF4351_BITPOS_CHRGPUMPCURSET);
	ADF4351_addSubReg(&data, (uint)dblBuf, ADF4351_BITPOS_DBLBUF);
	ADF4351_addSubReg(&data, (uint)R_count, ADF4351_BITPOS_RCOUNT);
	ADF4351_addSubReg(&data, (uint)R_div2, ADF4351_BITPOS_RDIV2);
	ADF4351_addSubReg(&data, (uint)refDbl, ADF4351_BITPOS_REFDBL);
	ADF4351_addSubReg(&data, (uint)muxOut, ADF4351_BITPOS_MUXOUT);
	ADF4351_addSubReg(&data, (uint)lowNoiseMode, ADF4351_BITPOS_LOWNOISEMODE);
	ADF4351_sendData(data);
}
void ADF4351_setRegister3(dByte clkDiv, byte clkDivMode, _bool CSR, _bool chrgCancel, byte ABP, byte bandSelClkMode)
{
	clkDiv &= MASK_12BIT;
	clkDivMode &= MASK_2BIT;
	CSR &= MASK_1BIT;
	chrgCancel &= MASK_1BIT;
	ABP &= MASK_1BIT;
	bandSelClkMode &= MASK_1BIT;
	uint data = ADF4351_CTRL_BITS_REG3;
	ADF4351_addSubReg(&data, (uint)clkDiv, ADF4351_BITPOS_CLKDIV);
	ADF4351_addSubReg(&data, (uint)clkDivMode, ADF4351_BITPOS_CLKDIVMODE);
	ADF4351_addSubReg(&data, (uint)CSR, ADF4351_BITPOS_CSR);
	ADF4351_addSubReg(&data, (uint)chrgCancel, ADF4351_BITPOS_CHRGCANCEL);
	ADF4351_addSubReg(&data, (uint)ABP, ADF4351_BITPOS_ABP);
	ADF4351_addSubReg(&data, (uint)bandSelClkMode, ADF4351_BITPOS_BANDSELCLKMODE);
	ADF4351_sendData(data);
}
void ADF4351_setRegister4(byte outPwr, _bool RF_out, byte auxOutPwr, _bool auxOut, byte auxOutSel,
		byte MTLD, byte VCO_pwrDown, byte bandSelClkDiv, byte RF_divSel, byte fdbckSel)
{
	outPwr &= MASK_2BIT;
	RF_out &= MASK_1BIT;
	auxOutPwr &= MASK_2BIT;
	auxOut &= MASK_1BIT;
	auxOutSel &= MASK_1BIT;
	MTLD &= MASK_1BIT;
	VCO_pwrDown &= MASK_1BIT;
	bandSelClkDiv &= MASK_8BIT;
	RF_divSel &= MASK_3BIT;
	fdbckSel &= MASK_1BIT;
	uint data = ADF4351_CTRL_BITS_REG4;
	ADF4351_addSubReg(&data, (uint)outPwr, ADF4351_BITPOS_OUTPWR);
	ADF4351_addSubReg(&data, (uint)RF_out, ADF4351_BITPOS_RFOUT);
	ADF4351_addSubReg(&data, (uint)auxOutPwr, ADF4351_BITPOS_AUXOUTPWR);
	ADF4351_addSubReg(&data, (uint)auxOut, ADF4351_BITPOS_AUXOUT);
	ADF4351_addSubReg(&data, (uint)auxOutSel, ADF4351_BITPOS_AUXOUTSEL);
	ADF4351_addSubReg(&data, (uint)MTLD, ADF4351_BITPOS_MTLD);
	ADF4351_addSubReg(&data, (uint)VCO_pwrDown, ADF4351_BITPOS_VCOPWRDOWN);
	ADF4351_addSubReg(&data, (uint)bandSelClkDiv, ADF4351_BITPOS_BANDSELCLKDIV);
	ADF4351_addSubReg(&data, (uint)RF_divSel, ADF4351_BITPOS_RFDIVSEL);
	ADF4351_addSubReg(&data, (uint)fdbckSel, ADF4351_BITPOS_FDBCKSEL);
	ADF4351_sendData(data);
}
void ADF4351_setRegister5(byte LD_pinMode)
{
	byte reserved = MASK_2BIT;
	LD_pinMode &= MASK_2BIT;
	uint data = ADF4351_CTRL_BITS_REG5;
	ADF4351_addSubReg(&data, (uint)reserved, ADF4351_BITPOS_REG5_RESERVED);
	ADF4351_addSubReg(&data, (uint)LD_pinMode, ADF4351_BITPOS_LDPINMODE);
	ADF4351_sendData(data);
}
void ADF4351_enable()
{
	HAL_GPIO_WritePin(ADF4351_CE_PORT, ADF4351_CE_PIN, GPIO_PIN_SET);
}
void ADF4351_disable()
{
	HAL_GPIO_WritePin(ADF4351_CE_PORT, ADF4351_CE_PIN, GPIO_PIN_RESET);
}
void ADF4351_settings(dByte INT, dByte FRAC, dByte MOD, dByte R_count, byte refDbl, byte R_DIV2, byte RF_div)
{
	byte LDF = ADF4351_LDF_FRAC;
	byte APB = ADF4351_ABP_6NS;
	byte psc = ADF4351_PSC_4v5;
	byte VCO_div = 1;
	if (FRAC == 0) {
		LDF = ADF4351_LDF_INT;
		APB = ADF4351_ABP_3NS;
	}
	if (ADF4351_FDBCKSEL == ADF4351_FDBCKSEL_DIVIDED)
		VCO_div = RF_div;
	if (ADF4351_VCO_freq(ADF4351_CLOCKREF, R_DIV2, refDbl, R_count, INT, FRAC, MOD) / (float)VCO_div > 3600.0f)
		psc = ADF4351_PSC_8v9;
	ADF4351_setRegister5(ADF4351_LDPINMODE_HIGH);
	HAL_Delay(SETTINGS_REG_DELAY_MS);
	ADF4351_setRegister4(ADF4351_OUTPWR_p5DBM, ADF4351_RFOUT_ENABLED, ADF4351_AUXOUTPWR_n1DBM, ADF4351_AUXOUT_DISABLED,
			ADF4351_AUXOUTSEL_FUNDAMENTAL, ADF4351_MTLD_DISABLED, ADF4351_VCOPWRDOWN_PWRUP,
			ADF4351_calcBandSelClkDivMax(ADF4351_CLOCKREF, R_count, refDbl, R_DIV2), RF_div, ADF4351_FDBCKSEL);
	HAL_Delay(SETTINGS_REG_DELAY_MS);
	ADF4351_setRegister3(1, ADF4351_CLKDIVMODE_OFF, ADF4351_CSR_DISABLED, ADF4351_CHRGCANCEL_DISABLED, APB, ADF4351_BANDSELCLKMODE_LOW);
	HAL_Delay(SETTINGS_REG_DELAY_MS);
	ADF4351_setRegister2(ADF4351_COUNTRST_DISABLED, ADF4351_CP3STATE_DISABLED, ADF4351_PWRDOWN_DISABLED, ADF4351_PDPOLARITY_POSITIVE,
			ADF4351_LDP_6NS, LDF, ADF4351_CHRGPUMPCURSET_2500U, ADF4351_DBLBUF_DISABLED, R_count, R_DIV2, refDbl, ADF4351_MUXOUT_DGND,
			ADF4351_LOWNOISEMODE_LOWNOISE);
	HAL_Delay(SETTINGS_REG_DELAY_MS);
	ADF4351_setRegister1(MOD, 1, psc, ADF4351_PHASEADJ_OFF);
	HAL_Delay(SETTINGS_REG_DELAY_MS);
	ADF4351_setRegister0(FRAC, INT);
	HAL_Delay(SETTINGS_REG_DELAY_MS);
}
float ADF4351_VCO_freq(float REF_in, _bool R_DIV2, _bool refDbl, dByte R_count, dByte INT, dByte FRAC, dByte MOD)
{
	return REF_in * (((float)(1 + refDbl)) / (((float)R_count) * ((float)(1 + R_DIV2)))) *
		   (((float)INT) + ((float)FRAC) / ((float)MOD));
}
float ADF4351_VCO_BS_freq(float REF_in, dByte R_count, byte refDbl, byte R_DIV2, byte bandSelClkDiv)
{
	return REF_in * (float)(1 + refDbl) / ((float)bandSelClkDiv * (float)R_count * (float)(1 + R_DIV2));
}
_bool isLockDetect()
{
	return (_bool)HAL_GPIO_ReadPin(ADF4351_LD_PORT, ADF4351_LD_PIN);
}


