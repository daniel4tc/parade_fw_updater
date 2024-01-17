/*
 * Copyright (c) Parade Technologies, Ltd. 2023.
 */

#include "report_data.h"

char* Touch_Type_Str[] = {
		[TOUCH_STANDARD_FINGER] = "Standard Finger",
		[TOUCH_PROXIMITY]       = "Proximity",
		[TOUCH_STYLUS]          = "Stylus",
		[TOUCH_HOVER]           = "Hover",
		[TOUCH_GLOVE]           = "Glove",
		[TOUCH_INVALID]         = "Invalid"
};

void output_debug_report(char* direction, char* format, char* label, char* type,
		ReportData* report)
{
	if (DEBUG == verbose_level_get()) {
		if (report == NULL) {
			output(WARNING, "%s: The 'report' arg is a NULL pointer.\n",
					__func__);
			return;
		} else if (report->len < 1) {
			output(WARNING,
					"%s: The report must be at least 1 byte long, but "
					"'report->len = 0'.\n",
					__func__);
			return;
		}

		uint str_index = 0;
		char* str = (char*) malloc(
				(report->len * TOUCH_REPORT_VALUE_HEX_FORMAT_STRLEN + 1)
				* sizeof(char));

		report->index = 0;
		while (report->index < report->len) {
			snprintf(
					&(str[str_index]),
					TOUCH_REPORT_VALUE_HEX_FORMAT_STRLEN + 1,
					"%02X ",
					report->data[report->index]
			);

			report->index++;
			str_index += TOUCH_REPORT_VALUE_HEX_FORMAT_STRLEN;
		}
		str[str_index - 1] = '\0';

		if (label == NULL) {
			output(DEBUG, "%s %s %s: '%s'\n",
					direction, format, type, str);
		} else {
			output(DEBUG, "%s %s %s %s: '%s'\n",
					direction, format, label, type, str);
		}

		free(str);
	}
}

void log_report_data(const ReportData* report, bool include_timestamp,
		char* label)
{
	if (report == NULL) {
		output(WARNING, "%s: The 'report' arg is a NULL pointer.\n", __func__);
		return;
	} else if (report->len < 1) {
		output(WARNING,
				"%s: The report must be at least 1 byte long, but "
				"'report->len = 0'.\n",
				__func__);
		return;
	}

	uint byte_index = 0;
	uint str_index = 0;
	char* str = (char*) malloc(
			(report->len * TOUCH_REPORT_VALUE_HEX_FORMAT_STRLEN + 1)
			* sizeof(char));

	while (byte_index < report->len) {
		snprintf(
				&(str[str_index]),
				TOUCH_REPORT_VALUE_HEX_FORMAT_STRLEN + 1,
				"%02X ",
				report->data[byte_index]
		);

		byte_index++;
		str_index += TOUCH_REPORT_VALUE_HEX_FORMAT_STRLEN;
	}
	str[str_index - 1] = '\0';

	if (include_timestamp && label != NULL) {
		struct timespec up_ts;
		float uptime;
		clock_gettime(CLOCK_MONOTONIC, &up_ts);
		uptime = (float) (up_ts.tv_sec + (up_ts.tv_nsec / (long double) 1e9));
		output(NOLEVEL_NOPREFIX, "[%13.6f] %s%s\n", uptime, label, str);
	} else if (include_timestamp) {
		struct timespec up_ts;
		float uptime;
		clock_gettime(CLOCK_MONOTONIC, &up_ts);
		uptime = (float) (up_ts.tv_sec + (up_ts.tv_nsec / (long double) 1e9));
		output(NOLEVEL_NOPREFIX, "[%13.6f] %s\n", uptime, str);
	} else if (label != NULL) {
		output(NOLEVEL_NOPREFIX, "%s%s\n", label, str);
	} else {
		output(NOLEVEL_NOPREFIX, "%s\n", str);
	}

	free(str);
}

int write_report(const ReportData* report, const char* filepath)
{
	output(DEBUG, "%s: Starting.\n", __func__);
	errno = 0;
	int rc = EXIT_SUCCESS;

	FILE* fptr = fopen(filepath, "wb");
	if (NULL == fptr) {
		rc = EXIT_FAILURE;
		goto END;
	}

	size_t bytes_written = fwrite(report->data, 1, report->len, fptr);
	if (ferror(fptr)) {
		output(DEBUG, "%s - %s\n", filepath, strerror(errno));
		rc = EXIT_FAILURE;
	}
	output(DEBUG, "Number of bytes written to %s: %u.\n", filepath,
			bytes_written);

	if (EOF == fclose(fptr)) {
		rc = EXIT_FAILURE;
	}

END:
	if (EXIT_FAILURE == rc) {
		output(ERROR, "%s: %s - %s\n", __func__, filepath, strerror(errno));
	}
	output(DEBUG, "%s: Leaving.\n", __func__);
	return rc;
}
