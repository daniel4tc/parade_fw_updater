/*
 * Copyright (c) Parade Technologies, Ltd. 2023.
 */

#include "fw_version.h"

int get_fw_version_from_flash(FW_Version* version)
{
	output(DEBUG, "%s: Starting.\n", __func__);
	PIP3_Rsp_Payload_GetSysinfo rsp;
	bool dut_state_stuck = false;

	/*
	 * Temporarily redirect stderr logs to /dev/null so the user does not see
	 * various ERROR messages for issues that suggest the firmware simply needs
	 * to be updated (e.g., PIP3 STATUS command does not work, or if the DUT is
	 * stuck in the ROM Bootloader). So instead, we just show the active
	 * firmware version as "0.0.0.0" to ensure/force a firmware update.
	 */
	fflush(stderr);
	int initial_stderr_fd = dup(STDERR_FILENO);
	int dev_null_fd = open("/dev/null", O_WRONLY);
	dup2(dev_null_fd, STDERR_FILENO);
	close(dev_null_fd);
	
	if (EXIT_SUCCESS != set_dut_state(DUT_STATE_TP_FW_SCANNING)) {
		dut_state_stuck = true;
		output(DEBUG,
			"Unable to get DUT into desired state (%s). This suggests that a "
			"firmware update is required.\n",
			DUT_STATE_LABELS[DUT_STATE_TP_FW_SCANNING]);
		version->major = 0;
		version->minor = 0;
		version->rev_control = 0;
		version->config_ver = 0;
	}

	fflush(stderr);
	dup2(initial_stderr_fd, STDERR_FILENO);
	close(initial_stderr_fd);

	if (dut_state_stuck) {
		return EXIT_SUCCESS;
		/* NOTREACHED */
	}

	if (EXIT_SUCCESS != do_pip3_get_sysinfo_cmd(0x00, &rsp)) {
		return EXIT_FAILURE;
		/* NOTREACHED */
	}

	version->major       = rsp.fw_major_version;
	version->minor       = rsp.fw_minor_version;
	version->rev_control = (
			   rsp.fw_rev_control_num[0]
			+ (rsp.fw_rev_control_num[1] << 8)
			+ (rsp.fw_rev_control_num[2] << 8 * 2)
			+ (rsp.fw_rev_control_num[3] << 8 * 3));
	version->config_ver  = (
			rsp.fw_config_version[0] + (rsp.fw_config_version[1] << 8));

    return EXIT_SUCCESS;
}

int get_fw_version_from_bin_header(const FW_Bin_Header* bin_header,
	FW_Version* version)
{
	output(DEBUG, "%s: Starting.\n", __func__);

	if (bin_header == NULL || version == NULL) {
		output(ERROR, "%s: NULL argument given.\n", __func__);
		return EXIT_FAILURE;
	}

	version->major       = bin_header->fw_major_version;
	version->minor       = bin_header->fw_minor_version;
	version->rev_control = (
		  (bin_header->fw_rev_control[0] << 8 * 3)
		+ (bin_header->fw_rev_control[1] << 8 * 2)
		+ (bin_header->fw_rev_control[2] << 8)
		+  bin_header->fw_rev_control[3]);
	version->config_ver = (
		(bin_header->config_version[0] << 8) +  bin_header->config_version[1]);

	return EXIT_SUCCESS;
}
