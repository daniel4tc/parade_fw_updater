/*
 * Copyright (c) Parade Technologies, Ltd. 2023.
 */

#include "pip3_self_test_id.h"

char* PIP3_SELF_TEST_NAMES[] = {
		[PIP3_SELF_TEST_ID_BIST]                     = (
				"BIST (Built In Self-Test)"),
		[PIP3_SELF_TEST_ID_SHORTS]                   = (
				"Shorts"),
		[PIP3_SELF_TEST_ID_OPENS]                    = (
				"Opens"),
		[PIP3_SELF_TEST_ID_SHORTS_NO_MASKS]          = (
				"Shorts test no masks"),
		[PIP3_SELF_TEST_ID_CM_PANEL]                 = (
				"CM panel (mutual cap)"),
		[PIP3_SELF_TEST_ID_CP_PANEL]                 = (
				"CP panel (self cap)"),
		[PIP3_SELF_TEST_ID_CM_BUTTONS]               = (
				"CM buttons"),
		[PIP3_SELF_TEST_ID_CP_BUTTONS]               = (
				"CP buttons"),
		[PIP3_SELF_TEST_ID_FORCE]                    = (
				"Force"),
		[PIP3_SELF_TEST_ID_OPEN_HIZ]                 = (
				"Open-HIZ test"),
		[PIP3_SELF_TEST_ID_OPEN_GND]                 = (
				"Open-GND test"),
		[PIP3_SELF_TEST_ID_CP_LFT_MODE]              = (
				"CP - LFT Mode"),
		[PIP3_SELF_TEST_ID_NOISE_SC]                 = (
				"Noise test for SC"),
		[PIP3_SELF_TEST_ID_NOISE_LFT]                = (
				"Noise test for LFT"),
		[PIP3_SELF_TEST_ID_CP_CHIP_ROUTING]          = (
				"CP - chip routing parasitic capacitance"),
		[PIP3_SELF_TEST_ID_NORM_RAW_COUNTS_PANEL]    = (
				"Normalized raw counts for panel"),
		[PIP3_SELF_TEST_ID_NORM_RAW_COUNTS_LFT_MODE] = (
				"Normalized raw counts for LFT mode"),
		[PIP3_SELF_TEST_ID_NOISE_MC]                 = (
				"Noise test for MC"),
		[PIP3_SELF_TEST_ID_DEVICE_OFF_I2C_BUS]       = (
				"Remove device off of the I2C bus for n seconds")
};
