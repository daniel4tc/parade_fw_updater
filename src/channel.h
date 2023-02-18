/*
 * Copyright (c) Parade Technologies, Ltd. 2023.
 */

#ifndef PTLIB_CHANNEL_H_
#define PTLIB_CHANNEL_H_

typedef enum {
	CHANNEL_TYPE_NONE,
	CHANNEL_TYPE_HIDRAW,
	CHANNEL_TYPE_I2CDEV,
	NUM_OF_CHANNEL_TYPES
} ChannelType;

extern char* CHANNEL_TYPE_NAMES[NUM_OF_CHANNEL_TYPES];

#endif 
