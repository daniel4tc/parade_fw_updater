/*
 * Copyright (c) Parade Technologies, Ltd. 2023.
 */

#include "channel.h"

char* CHANNEL_TYPE_NAMES[] = {
		[CHANNEL_TYPE_NONE]   = "No channel type selected",
		[CHANNEL_TYPE_HIDRAW] = "HIDRAW",
		[CHANNEL_TYPE_I2CDEV] = "I2C-DEV",
		[CHANNEL_TYPE_TTDL]   = "TTDL",
};
