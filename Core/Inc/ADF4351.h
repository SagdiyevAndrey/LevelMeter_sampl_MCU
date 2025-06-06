#ifndef ADF4351_H
#define ADF4351_H

#include "comDef.h"

#define ADF4351_CLOCKREF				50.0f // MHz

// Multiplexer output (unused)
#define ADF4351_MUXOUT_PORT				GPIOC
#define ADF4351_MUXOUT_PIN				GPIO_PIN_1
// Lock detect output pin
#define ADF4351_LD_PORT					GPIOA
#define ADF4351_LD_PIN					GPIO_PIN_2
// Serial clock input
#define ADF4351_CLK_PORT				GPIOA
#define ADF4351_CLK_PIN					GPIO_PIN_1
// Serial data input
#define ADF4351_DATA_PORT				GPIOA
#define ADF4351_DATA_PIN				GPIO_PIN_0
// Load enable (input)
#define ADF4351_LE_PORT					GPIOC
#define ADF4351_LE_PIN					GPIO_PIN_3
// Chip enable (input)
#define ADF4351_CE_PORT					GPIOC
#define ADF4351_CE_PIN					GPIO_PIN_2

#define ADF4351_PSC_4v5					0
#define ADF4351_PSC_8v9					1
#define ADF4351_PHASEADJ_OFF			0
#define ADF4351_PHASEADJ_ON				1
#define ADF4351_COUNTRST_DISABLED		0
#define ADF4351_COUNTRST_ENABLED		1
#define ADF4351_CP3STATE_DISABLED		0
#define ADF4351_CP3STATE_ENABLED		1
#define ADF4351_PWRDOWN_DISABLED		0
#define ADF4351_PWRDOWN_ENABLED			1
#define ADF4351_PDPOLARITY_NEGATIVE		0
#define ADF4351_PDPOLARITY_POSITIVE		1
#define ADF4351_LDP_10NS				0
#define ADF4351_LDP_6NS					1
#define ADF4351_LDF_FRAC				0
#define ADF4351_LDF_INT					1
#define ADF4351_CHRGPUMPCURSET_0310U	0x0
#define ADF4351_CHRGPUMPCURSET_0630U	0x1
#define ADF4351_CHRGPUMPCURSET_0940U	0x2
#define ADF4351_CHRGPUMPCURSET_1250U	0x3
#define ADF4351_CHRGPUMPCURSET_1560U	0x4
#define ADF4351_CHRGPUMPCURSET_1880U	0x5
#define ADF4351_CHRGPUMPCURSET_2190U	0x6
#define ADF4351_CHRGPUMPCURSET_2500U	0x7
#define ADF4351_CHRGPUMPCURSET_2810U	0x8
#define ADF4351_CHRGPUMPCURSET_3130U	0x9
#define ADF4351_CHRGPUMPCURSET_3440U	0xA
#define ADF4351_CHRGPUMPCURSET_3750U	0xB
#define ADF4351_CHRGPUMPCURSET_4060U	0xC
#define ADF4351_CHRGPUMPCURSET_4380U	0xD
#define ADF4351_CHRGPUMPCURSET_4690U	0xE
#define ADF4351_CHRGPUMPCURSET_5000U	0xF
#define ADF4351_DBLBUF_DISABLED			0
#define ADF4351_DBLBUF_ENABLED			1
#define ADF4351_RDIV2_DISABLED			0
#define ADF4351_RDIV2_ENABLED			1
#define ADF4351_REFDBL_DISABLED			0
#define ADF4351_REFDBL_ENABLED			1
#define ADF4351_MUXOUT_3STATE			0
#define ADF4351_MUXOUT_DVDD				1
#define ADF4351_MUXOUT_DGND				2
#define ADF4351_MUXOUT_RCOUNT			3
#define ADF4351_MUXOUT_NDIV				4
#define ADF4351_MUXOUT_ANALOGLD			5
#define ADF4351_MUXOUT_DIGITALLD		6
#define ADF4351_LOWNOISEMODE_LOWNOISE	0
#define ADF4351_LOWNOISEMODE_LOWSPUR	3
#define ADF4351_CLKDIVMODE_OFF			0
#define ADF4351_CLKDIVMODE_FASTLOCK		1
#define ADF4351_CLKDIVMODE_RESYNC		2
#define ADF4351_CSR_DISABLED			0
#define ADF4351_CSR_ENABLED				1
#define ADF4351_CHRGCANCEL_DISABLED		0
#define ADF4351_CHRGCANCEL_ENABLED		1
#define ADF4351_ABP_6NS					0
#define ADF4351_ABP_3NS					1
#define ADF4351_BANDSELCLKMODE_LOW		0
#define ADF4351_BANDSELCLKMODE_HIGH		1
#define ADF4351_OUTPWR_n4DBM			0
#define ADF4351_OUTPWR_n1DBM			1
#define ADF4351_OUTPWR_p2DBM			2
#define ADF4351_OUTPWR_p5DBM			3
#define ADF4351_RFOUT_DISABLED			0
#define ADF4351_RFOUT_ENABLED			1
#define ADF4351_AUXOUTPWR_n4DBM			0
#define ADF4351_AUXOUTPWR_n1DBM			1
#define ADF4351_AUXOUTPWR_p2DBM			2
#define ADF4351_AUXOUTPWR_p5DBM			3
#define ADF4351_AUXOUT_DISABLED			0
#define ADF4351_AUXOUT_ENABLED			1
#define ADF4351_AUXOUTSEL_DIVIDEDOUT	0
#define ADF4351_AUXOUTSEL_FUNDAMENTAL	1
#define ADF4351_MTLD_DISABLED			0
#define ADF4351_MTLD_ENABLED			1
#define ADF4351_VCOPWRDOWN_PWRUP		0
#define ADF4351_VCOPWRDOWN_PWRDOWN		1
#define ADF4351_RFDIVSEL_1				0
#define ADF4351_RFDIVSEL_2				1
#define ADF4351_RFDIVSEL_4				2
#define ADF4351_RFDIVSEL_8				3
#define ADF4351_RFDIVSEL_16				4
#define ADF4351_RFDIVSEL_32				5
#define ADF4351_RFDIVSEL_64				6
#define ADF4351_FDBCKSEL_DIVIDED		0
#define ADF4351_FDBCKSEL_FUNDAMENTAL	1
#define ADF4351_LDPINMODE_LOW			0
#define ADF4351_LDPINMODE_DIGITALLD		1
#define ADF4351_LDPINMODE_HIGH			3

#define ADF4351_VCO_BANDSEL_FREQ_MAX	0.125f // MHz

#define ADF4351_FDBCKSEL				ADF4351_FDBCKSEL_FUNDAMENTAL

#define MHZ								1000000

void ADF4351_initPorts();
void ADF4351_init();
void ADF4351_sendData(uint data);
void ADF4351_setRegister0(dByte fracValue, dByte intValue); // 12-bit fractional value; 16-bit integer value
void ADF4351_setRegister1(dByte modValue, dByte phaseValue, byte psc, _bool phaseAdj); /* 12-bit modulus value; 12-bit phase value;
 	 prescaler (0 - 4/5, 1 - 8/9);
 	 phase adjust (0 - off, 1 - on) */
void ADF4351_setRegister2(_bool countRst, _bool CP3state, _bool pwrDown, byte PD_polarity, byte LDP, byte LDF, byte chrgPumpCurSet,
		byte dblBuf, dByte R_count, _bool R_div2, _bool refDbl, byte muxOut, byte lowNoiseMode); /*
		counter reset (0 - disabled, 1 - enabled);
		CP three-state (0 - disabled, 1 - enabled);
		power-down (0 - disabled, 1 - enabled);
		PD polarity (0 - negative, 1 - positive);
		LDP (0 - 10ns, 1 - 6ns);
		LDF (0 - FRAC-N, 1 - INT-N);
		charge pump current settings (0 - 0.31mA, 1 - 0.63mA, 2 - 0.94mA, 3 - 1.25mA,
									  4 - 1.56mA, 5 - 1.88mA, 6 - 2.19mA, 7 - 2.50mA,
									  8 - 2.81mA, 9 - 3.13mA, A - 3.44mA, B - 3.75mA,
									  C - 4.06mA, D - 4.38mA, E - 4.69mA, F - 5.00mA);
		double buffer (0 - disabled, 1 - enabled);
		10-bit R counter;
		R divisor by 2 (0 - disabled, 1 - enabled);
		doubler (0 - disabled, 1 - enabled);
		multiplexer output (0 - 3-state output, 1 - DVdd, 2 - DGND, 3 - R counter,
							4 - N divider, 5 - analog lock detect, 6 - digital lock detect);
		low noise mode (0 - low noise mode, 3 - low spur mode) */
void ADF4351_setRegister3(dByte clkDiv, byte clkDivMode, _bool CSR, _bool chrgCancel, byte ABP, byte bandSelClkMode); /*
		12-bit clock divider value;
		clock divider mode (0 - clock divider off, 1 - fast lock enable, 2 - resync enable);
		cycle slip reduction (0 - disabled, 1 - enabled);
 	 	charge cancelation (0 - disabled, 1 - enabled);
 	 	antibacklash pulse width (0 - 6ns(FRAC-N), 1 - 3ns(INT-N));
 	 	band select clock mode (0 - low, 1 - high) */
void ADF4351_setRegister4(byte outPwr, _bool RF_out, byte auxOutPwr, _bool auxOut, byte auxOutSel,
		byte MTLD, byte VCO_pwrDown, byte bandSelClkDiv, byte RF_divSel, byte fdbckSel); /*
		output power (0 - -4dBm, 1 - -1dBm, 2 - 2dBm, 3 - 5dBm);
		RF output (0 - disabled, 1 - enabled);
		auxiliary output power (0 - -4dBm, 1 - -1dBm, 2 - 2dBm, 3 - 5dBm);
		auxiliary output (0 - disabled, 1 - enabled);
		auxiliary output select (0 - divided output, 1 - fundamental);
		mute till lock detect (0 - mute disabled, 1 - mute enabled);
		VCO power-down (0 - VCO powered up, 1 - VCO powered down);
		8-bit band select clock divider;
		RF divider select (0 - 1, 1 - 2, 2 - 4, 3 - 8, 4 - 16, 5 - 32, 6 - 64);
		feedback select (0 - divided, 1 - fundamental) */
void ADF4351_setRegister5(byte LD_pinMode); // lock detect pin mode (0 - low, 1 - digital lock detect, 2 - low, 3 - high)
void ADF4351_enable();
void ADF4351_disable();
void ADF4351_settings(dByte INT, dByte FRAC, dByte MOD, dByte R_count, byte refDbl, byte R_DIV2, byte RF_div);
byte ADF4351_RF_div_val(byte RF_div);
float ADF4351_VCO_freq(float REF_in, _bool R_DIV2, _bool refDbl, dByte R_count, dByte INT, dByte FRAC, dByte MOD); /* Calculate VCO frequency in [MHz]
 	 	REF_in - input frequency [MHz]
 	 	other arguments - registers values*/
float ADF4351_VCO_BS_freq(float REF_in, dByte R_count, byte refDbl, byte R_DIV2, byte bandSelClkDiv); /* Calculate clock frequency for band selection in VCO in [MHz]
		REF_in - input frequency [MHz]
 	 	other arguments - registers values*/
_bool isLockDetect();

#endif
