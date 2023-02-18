/*
 * Copyright (c) Parade Technologies, Ltd. 2023.
 */

#include "dut_state.h"

char* DUT_STATE_LABELS[] = {
		[DUT_STATE_INVALID]               = (
				"Invalid DUT State"
				),
		[DUT_STATE_TP_FW_BOOT]            = (
				"Touch Processor, Firmware Exec, Boot Mode"
				),
		[DUT_STATE_TP_FW_SCANNING]        = (
				"Touch Processor, Firmware Exec, Scanning Mode"
				),
		[DUT_STATE_TP_FW_DEEP_SLEEP]      = (
				"Touch Processor, Firmware Exec, Deep Sleep Mode"
				),
		[DUT_STATE_TP_FW_TEST]            = (
				"Touch Processor, Firmware Exec, Test Mode"
				),
		[DUT_STATE_TP_FW_DEEP_STANDBY]    = (
				"Touch Processor, Firmware Exec, Deep Standby Mode"
				),
		[DUT_STATE_TP_FW_SECONDARY_IMAGE] = (
				"Touch Processor, Firmware Exec, Secondary Image Programming "
				"Mode"),
		[DUT_STATE_TP_FW_SYS_MODE_ANY]    = (
				"Touch Processor, Firmware Exec (any System Mode)"
				),
		[DUT_STATE_TP_BL]                 = (
				"Touch Processor, Bootloader Exec"
				),
		[DUT_STATE_AUX_MCU_FW]            = (
				"AUX MCU, Firmware Exec"
				),
		[DUT_STATE_AUX_MCU_BL]            = (
				"AUX MCU, Bootloader Exec"
				),
		[DUT_STATE_DEFAULT]               = (
				"Default DUT State (not ready for communication)"
				)
};

char* DUT_EXEC_LABELS[] = {
		[DUT_EXEC_INVALID] = "Invalid DUT Exec",
		[DUT_EXEC_BL]      = "Bootloader Exec",
		[DUT_EXEC_FW]      = "Firmware Exec"
};
