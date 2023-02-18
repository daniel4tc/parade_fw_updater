/*
 * Copyright (c) Parade Technologies, Ltd. 2023.
 */

#include "pip3_status_code.h"

char* PIP3_STATUS_CODE_LABELS[] = {
		[PIP3_STATUS_CODE_SUCCESS]                 = (
				"Successful Completion of Command"),

		[PIP3_STATUS_CODE_FAILED]                  = (
				"Command Failed to Complete"),

		[PIP3_STATUS_CODE_INIT_FAILURE]            = (
				"Initialization Failure"),

		[PIP3_STATUS_CODE_FILE_ALREADY_OPEN]       = (
				"File Already Open"),

		[PIP3_STATUS_CODE_FILE_NOT_OPEN]           = (
				"File Not Open"),

		[PIP3_STATUS_CODE_IO_FAILURE]              = (
				"I/O Failure"),

		[PIP3_STATUS_CODE_UNKNOWN_IOCTL]           = (
				"Unknown IOCTL"),

		[PIP3_STATUS_CODE_BAD_ADDRESS]             = (
				"Bad Address"),

		[PIP3_STATUS_CODE_BAD_FILE_NAME]           = (
				"Bad File Name"),

		[PIP3_STATUS_CODE_EOF]                     = (
				"End of File"),

		[PIP3_STATUS_CODE_TOO_MANY_FILES]          = (
				"Too Many Files"),

		[PIP3_STATUS_CODE_TIMEOUT]                 = (
				"Timeout"),

		[PIP3_STATUS_CODE_ABORTED]                 = (
				"Aborted"),

		[PIP3_STATUS_CODE_BAD_CRC]                 = (
				"Bad CRC"),

		[PIP3_STATUS_CODE_UNKNOWN_RECORD_TYPE]     = (
				"Unknown Record Type"),

		[PIP3_STATUS_CODE_BAD_FRAME]               = (
				"Bad Frame"),

		[PIP3_STATUS_CODE_NO_PERMISSION]           = (
				"No Permission"),

		[PIP3_STATUS_CODE_UNKNOWN_CMD]             = (
				"Unknown Command"),

		[PIP3_STATUS_CODE_INVALID_PARAMS]          = (
				"Invalid Parameters"),

		[PIP3_STATUS_CODE_IO_ALREADY_ACTIVE]       = (
				"I/O Already Active"),

		[PIP3_STATUS_CODE_IO_ABORTED_FOR_SHUTDOWN] = (
				"I/O Aborted due to Shutdown"),

		[PIP3_STATUS_CODE_INVALID_IMAGE]           = (
				"Invalid Image"),

		[PIP3_STATUS_CODE_UNKNOWN_REGISTER]        = (
				"Unknown Register"),

		[PIP3_STATUS_CODE_BAD_LENGTH]              = (
				"Bad Length"),

		[PIP3_STATUS_CODE_TRIM_FAILURE]            = (
				"Trim Failure"),

		[PIP3_STATUS_CODE_CONFIG_DATA_ERROR]       = (
				"Configuration Data Error"),

		[PIP3_STATUS_CODE_CAL_DATA_ERROR]          = (
				"Calibration Data Error"),

		[PIP3_STATUS_CODE_INCOMBATIBLE_DDI_STATE]  = (
				"Incompatible DDI State or State Transition Encountered During "
				"Command"),
		[PIP3_STATUS_CODE_INCORRECT_SYS_MODE]      = (
				"Incorrect System Mode to Execute Command")

};
