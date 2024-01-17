/*
 * Copyright (c) Parade Technologies, Ltd. 2023.
 */

#ifndef PTLIB_PIP_PIP3_SELF_TEST_ID_H_
#define PTLIB_PIP_PIP3_SELF_TEST_ID_H_

typedef enum {
	PIP3_SELF_TEST_ID_BIST                     = 0x01,
	PIP3_SELF_TEST_ID_SHORTS                   = 0x02,
	PIP3_SELF_TEST_ID_OPENS                    = 0x03,
	PIP3_SELF_TEST_ID_SHORTS_NO_MASKS          = 0x04,
	PIP3_SELF_TEST_ID_CM_PANEL                 = 0x05,
	PIP3_SELF_TEST_ID_CP_PANEL                 = 0x06,
	PIP3_SELF_TEST_ID_CM_BUTTONS               = 0x07,
	PIP3_SELF_TEST_ID_CP_BUTTONS               = 0x08,
	PIP3_SELF_TEST_ID_FORCE                    = 0x09,
	PIP3_SELF_TEST_ID_OPEN_HIZ                 = 0x0A,
	PIP3_SELF_TEST_ID_OPEN_GND                 = 0x0B,
	PIP3_SELF_TEST_ID_CP_LFT_MODE              = 0x0C,
	PIP3_SELF_TEST_ID_NOISE_SC                 = 0x0D,
	PIP3_SELF_TEST_ID_NOISE_LFT                = 0X0E,
	PIP3_SELF_TEST_ID_CP_CHIP_ROUTING          = 0x0F,
	PIP3_SELF_TEST_ID_NORM_RAW_COUNTS_PANEL    = 0x10,
	PIP3_SELF_TEST_ID_NORM_RAW_COUNTS_LFT_MODE = 0x11,
	PIP3_SELF_TEST_ID_NOISE_MC                 = 0x12,
	PIP3_SELF_TEST_ID_SENSOR_CA_NOISE          = 0x13,
	PIP3_SELF_TEST_ID_TOUCH_PANEL_ENABLED      = 0x14,

	NUM_OF_PIP3_SELF_TEST_IDS = 21
} PIP3_Self_Test_ID;

extern char* PIP3_SELF_TEST_NAMES[NUM_OF_PIP3_SELF_TEST_IDS];

typedef enum {
	PIP3_DATA_FORMAT_ID_2BYTE_UNSIGNED            = 0x0,
	PIP3_DATA_FORMAT_ID_2BYTE_UNSIGNED_PLUS_EXTRA = 0x1,
	PIP3_DATA_FORMAT_ID_2BYTE_SIGNED              = 0x2,

	NUM_OF_PIP3_DATA_FORMAT_IDS = 3
} PIP3_Data_Format_ID;

typedef enum {
	PIP3_DATA_UNIT_ID_NANOAMPS            = 0x0, 
	PIP3_DATA_UNIT_ID_100THS_OF_PICOFARAD = 0x1, 
	PIP3_DATA_UNIT_ID_COUNTS              = 0x2, 
	PIP3_DATA_UNIT_ID_PASS_OR_FAIL        = 0x3, 

	NUM_OF_PIP3_DATA_UNIT_IDS = 4
} PIP3_Data_Unit_ID;

#endif 
