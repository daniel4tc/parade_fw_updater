/*
 * Copyright (c) Parade Technologies, Ltd. 2023.
 */

#ifndef PTLIB_BASE64_H_
#define PTLIB_BASE64_H_

#include <stdint.h>
#include <stdlib.h>
// #include <b64/cdecode.h>
#include "logging.h"

typedef struct {
	uint8_t* data;
	size_t len;
} ByteData;

// extern int b64_decode(const char* b64_str, ByteData* bin_array);

#endif 
