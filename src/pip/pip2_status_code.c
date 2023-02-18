/*
 * Copyright (c) Parade Technologies, Ltd. 2023.
 */

#include "pip2_status_code.h"

char* PIP2_STATUS_CODE_LABELS[] = {
		[PIP2_STATUS_CODE_SUCCESS]                 = (
				"Successful Completion of Command"),

		[PIP2_STATUS_CODE_BUSY]                    = (
				"Busy"),

		[PIP2_STATUS_CODE_INIT_FAILURE]            = (
				"Initialization Failure"),

		[PIP2_STATUS_CODE_FILE_ALREADY_OPEN]       = (
				"File Already Open"),

		[PIP2_STATUS_CODE_FILE_NOT_OPEN]           = (
				"File Not Open"),

		[PIP2_STATUS_CODE_IO_FAILURE]              = (
				"I/O Failure"),

		[PIP2_STATUS_CODE_UNKNOWN_IOCTL]           = (
				"Unknown IOCTL"),

		[PIP2_STATUS_CODE_BAD_ADDRESS]             = (
				"Bad Address"),

		[PIP2_STATUS_CODE_BAD_FILE_NAME]           = (
				"Bad File Name"),

		[PIP2_STATUS_CODE_EOF]                     = (
				"End of File"),

		[PIP2_STATUS_CODE_TOO_MANY_FILES]          = (
				"Too Many Files"),

		[PIP2_STATUS_CODE_TIMEOUT]                 = (
				"Timeout"),

		[PIP2_STATUS_CODE_ABORTED]                 = (
				"Aborted"),

		[PIP2_STATUS_CODE_BAD_CRC]                 = (
				"Bad CRC"),

		[PIP2_STATUS_CODE_UNKNOWN_RECORD_TYPE]     = (
				"Unknown Record Type"),

		[PIP2_STATUS_CODE_BAD_FRAME]               = (
				"Bad Frame"),

		[PIP2_STATUS_CODE_NO_PERMISSION]           = (
				"No Permission"),

		[PIP2_STATUS_CODE_UNKNOWN_CMD]             = (
				"Unknown Command"),

		[PIP2_STATUS_CODE_INVALID_PARAMS]          = (
				"Invalid Parameters"),

		[PIP2_STATUS_CODE_IO_ALREADY_ACTIVE]       = (
				"I/O Already Active"),

		[PIP2_STATUS_CODE_IO_ABORTED_FOR_SHUTDOWN] = (
				"I/O Aborted due to Shutdown"),

		[PIP2_STATUS_CODE_INVALID_IMAGE]           = (
				"Invalid Image"),

		[PIP2_STATUS_CODE_UNKNOWN_REGISTER]        = (
				"Unknown Register"),

		[PIP2_STATUS_CODE_BAD_LENGTH]              = (
				"Bad Length"),

		[PIP2_STATUS_CODE_TRIM_FAILURE]            = (
				"Trim Failure"),
};
