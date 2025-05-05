#ifndef COMDEF_H
#define COMDEF_H

#include <math.h>
#include <string.h>
#include "stm32u5xx_hal.h"

#define byte 				unsigned char
#define dByte 				unsigned short
#define uint 				unsigned int
#define _bool				byte

#define TRUE				1
#define FALSE				0
#define PULSE_HIGH			1
#define PULSE_LOW			0

#define MASK_BIT_0			0x00000001
#define MASK_BIT_1			0x00000002
#define MASK_BIT_2			0x00000004
#define MASK_BIT_3			0x00000008
#define MASK_BIT_4			0x00000010
#define MASK_BIT_5			0x00000020
#define MASK_BIT_6			0x00000040
#define MASK_BIT_7			0x00000080
#define MASK_BIT_8			0x00000100
#define MASK_BIT_9			0x00000200
#define MASK_BIT_10			0x00000400
#define MASK_BIT_11			0x00000800
#define MASK_BIT_12			0x00001000
#define MASK_BIT_13			0x00002000
#define MASK_BIT_14			0x00004000
#define MASK_BIT_15			0x00008000

#endif
