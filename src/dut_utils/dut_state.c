/*
 * Copyright (c) Parade Technologies, Ltd. 2023.
 */

#include "dut_state.h"

char* DUT_STATE_LABELS[] = {
		[DUT_STATE_INVALID]                     = (
				"Invalid DUT State"
				),
		[DUT_STATE_TP_FW_BOOT]                  = (
				"Touch Processor, Firmware Exec, Boot Mode"
				),
		[DUT_STATE_TP_FW_SCANNING]              = (
				"Touch Processor, Firmware Exec, Scanning Mode"
				),
		[DUT_STATE_TP_FW_DEEP_SLEEP]            = (
				"Touch Processor, Firmware Exec, Deep Sleep Mode"
				),
		[DUT_STATE_TP_FW_TEST]                  = (
				"Touch Processor, Firmware Exec, Test Mode"
				),
		[DUT_STATE_TP_FW_DEEP_STANDBY]          = (
				"Touch Processor, Firmware Exec, Deep Standby Mode"
				),
		[DUT_STATE_TP_FW_PROGRAMMER_IMAGE]      = (
				"Touch Processor, Firmware Exec, Programmer Image Mode"
				),
		[DUT_STATE_TP_FW_SYS_MODE_ANY]          = (
				"Touch Processor, Firmware Exec (any System Mode)"
				),
		[DUT_STATE_TP_BL]                       = (
				"Touch Processor, Bootloader Exec"
				),
		[DUT_STATE_AUX_MCU_FW_UTILITY_IMAGE]                  = (
				"AUX MCU, Firmware Exec"
				),
		[DUT_STATE_AUX_MCU_FW_PROGRAMMER_IMAGE] = (
				"AUX MCU, Firmware Exec, Programmer Image Mode"
				),
		[DUT_STATE_DEFAULT]                     = (
				"Default DUT State (not ready for communication)"
				)
};

char* DUT_EXEC_LABELS[] = {
		[DUT_EXEC_INVALID] = "Invalid DUT Exec",
		[DUT_EXEC_BL]      = "Bootloader Exec",
		[DUT_EXEC_FW]      = "Firmware Exec"
};

static uint8_t aux_mcu_active_duration_seconds = 0;

void set_aux_mcu_active_duration_seconds(uint8_t duration)
{
	output(DEBUG, "%s: Starting.\n", __func__);
	aux_mcu_active_duration_seconds = duration;
}

uint8_t get_aux_mcu_active_duration_seconds()
{
	output(DEBUG, "%s: Starting.\n", __func__);
	return aux_mcu_active_duration_seconds;
}
