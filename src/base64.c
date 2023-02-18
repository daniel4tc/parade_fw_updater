/*
 * Copyright (c) Parade Technologies, Ltd. 2023.
 */

#include "base64.h"

int b64_decode(const char* b64_str, ByteData* bin_array)
{
	output(DEBUG, "%s: Starting.\n", __func__);

	size_t b64_strlen = strlen(b64_str);
	bin_array->data = malloc(b64_strlen); 
	if (NULL == bin_array->data) {
		output(ERROR, "%s: Memory allocation failed.\n", __func__);
		return EXIT_FAILURE;
	}

	base64_decodestate ds;
	base64_init_decodestate(&ds);

	bin_array->len = base64_decode_block(b64_str, b64_strlen,
			(char*) bin_array->data, &ds);

	output(DEBUG, "%s: Returning.\n", __func__);
	return EXIT_SUCCESS;
}

