/*
 * Copyright (c) Parade Technologies, Ltd. 2023.
 */
#ifndef _FW_VERSION_H
#define _FW_VERSION_H

#include "base64.h"
#include "dut_utils/dut_utils.h"

typedef struct {
	int major;
	int minor;
	int rev_control;
	int config_ver;
	int silicon_id;
} FW_Version;

extern int get_fw_version_from_flash(FW_Version* version);
extern int get_fw_version_from_bin_header(const FW_Bin_Header* bin_header,
	FW_Version* version);

#endif /* _FW_VERSION_H */

