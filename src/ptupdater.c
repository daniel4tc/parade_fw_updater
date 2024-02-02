/*
 * Copyright (c) Parade Technologies, Ltd. 2023.
 */
#include <getopt.h>
#include "dut_utils/dut_utils.h"
#include "fw_version.h"
#include "hid/hidraw.h"
// #include "ptu_parse.h"

#define SW_VERSION "0.5.0"
#define FLAG_SET 1
#define FLAG_NOT_SET 0

#define I2C_ADDR 0x24

typedef struct {
	bool check_active;
	bool check_target;
	bool update;
	char* hidraw_sysfs_node_file;
	char* ptu_file;
	bool use_i2c_dev;
	int i2c_bus;
	int i2c_addr;
} PtUpdater_Config;

static void _parse_args(int argc, char **argv, PtUpdater_Config* config);
static void _print_help();
static int _run(const PtUpdater_Config* config);
static int _setup(const PtUpdater_Config* config);

int main(int argc, char **argv)
{
	int rc = EXIT_FAILURE;

	PtUpdater_Config config = {
		.check_active = false,
		.check_target = false,
		.update = false,
		.hidraw_sysfs_node_file = NULL,
		.ptu_file = NULL,
		.use_i2c_dev = false,
		.i2c_bus = 0,
		.i2c_addr = I2C_ADDR,
	};

	if (argc == 1) {
		_print_help();
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	} else if (argv[1][0] != '-') {
		/*
		 * If the first argument doesn't have a CLI flag, then it is assumed to
		 * be the HIDRAW sysfs node file path.
		 */
		config.hidraw_sysfs_node_file = argv[1];
	}

	/*
	 * Process Option Inputs
	 * =====================================================================
	 *
	 * All option inputs are processed first. This means that boolean option
	 * flags will be set, and arguments validated and stored in variables
	 * for use later. No actions, even as simple as printing help text, are
	 * taken during the option processing.
	 */
	_parse_args(argc, argv, &config);

	if (config.hidraw_sysfs_node_file == NULL) {
		output(FATAL,
			"Must provide the HIDRAW node path as the first argument.\n");
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	output(DEBUG, "HIDRAW sysfs node filepath: '%s'.\n",
		config.hidraw_sysfs_node_file);

	/*
	 * Act On Arguments
	 * ========================================================================
	 *
	 * Now that all command line arguments have been processed they can now
	 * be acted on. The order of execution is important here. For example,
	 * the help flag blocks all other actions, therefore, it will be checked
	 * first and if it exists the process will exit.
	 */	

	output(INFO, "\tptupdater v%s\n", SW_VERSION);
	
	/*
	 * The '--check-target' CLI option does not require any communication with 
     * the DUT. So unless the '--check-active' and/or '--update' options, there
	 * is no need to initialize the HIDRAW and PIP3 APIs.
	 */
	if ((config.check_active || config.update)
			&& EXIT_SUCCESS != _setup(&config)) {
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	rc = _run(&config);

	exit(rc);
}

static void _parse_args(int argc, char **argv, PtUpdater_Config* config)
{
	bool help_flag = false;
	bool version_flag = false;
	bool verbosity_flag = false;

	/*
	 * Process Option Inputs
	 * =====================================================================
	 *
	 * All option inputs are processed first. This means that boolean option
	 * flags will be set, and arguments validated and stored in variables
	 * for use later. No actions, even as simple as printing help text, are
	 * taken during the option processing.
	 */
	while (1) {
		int c;
		static struct option long_options[] = {
			/*
			 * Long options that set a flag and do not have an
			 * argument. If the option has both short and long
			 * forms the next section must be used.
			 */
			{"check-active", no_argument, 0, },
			{"version",      no_argument, 0, },

			/*
			 * Options that set a flag which have both short and
			 * long forms. Make sure that each of the single
			 * character flags are listed in the getopt_long
			 * function call below.
			 */
			{"help", no_argument, 0, 'h'},

			/*
			 * Options requiring arguments.
			 */
			{"check-target", required_argument, 0, },
			{"i2c-bus",      required_argument, 0, },
			{"update", 	     required_argument, 0, },
			{"verbose",      required_argument, 0, },
	
			/*
			 * getopt_long requires this structure to be terminated
			 * with an element containing all zeros.
			 */
			{0, 0, 0, 0}
		};

		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long(argc, argv, "h", long_options, &option_index);
		/* Detect the end of the options and exit the while loop.  */
		if (c == -1) {
			break;
		}

		switch (c) {
		case 0:
			if (strcmp(long_options[option_index].name, "check-active") == 0) {
				config->check_active = true;
				output(DEBUG, "option --check-active\n");
			} else if (strcmp(long_options[option_index].name, "i2c-bus")
					== 0) {
				config->use_i2c_dev = true;
				config->i2c_bus = (int) strtol(optarg, NULL, 10);
				output(DEBUG, "option --i2c-bus %d\n", config->i2c_bus);
			} else if (strcmp(long_options[option_index].name, "check-target")
					== 0) {
				config->check_target = true;
				config->ptu_file = optarg;
				output(DEBUG, "option --check-target %s\n", config->ptu_file);
			} else if (strcmp(long_options[option_index].name, "update") == 0) {
				config->update = true;
				config->ptu_file = optarg;
				output(DEBUG, "option --update %s\n", config->ptu_file);
			} else if (strcmp(long_options[option_index].name, "verbose")
					== 0) {
				output(INFO, "A.14\n");
				verbose_level_set((int)strtol(optarg, NULL, 10));
				output(DEBUG, "Verbose level set to: %d\n",
					verbose_level_get());
				verbosity_flag = true; 
			} else if (strcmp(long_options[option_index].name, "version")
					== 0) {
				version_flag = true;
			}
			break;

		case 'h':
			/* -h and --help */
			output(DEBUG, "option -h, --help\n");
			help_flag = true;
			break;

		case '?':
			/*
			 * Invalid Option
			 * getopt_long already printed an error message. This case statement
			 * is here just for clarity.
			 */
			abort();
			/* NOTREACHED */
		case ':':
			/*
			 * Missing Option Argument
			 * getopt_long already printed an error message. This case statement
			 * is here just for clarity.
			 */
			break;

		default:
			_print_help();
			output(FATAL, "Unexpected input! Printed help.\n");
			abort();
			/* NOTREACHED */
		}
	}

	/*
	 * If the help flag is set print the help and then exit. Any other
	 * arguments given are ignored. 
	 */
	if (help_flag) {
		_print_help();
		exit(EXIT_SUCCESS);
		/* NOTREACHED */
	}

	/*
	 * If the version flag is set print the version and then exit. Any other
	 * arguments given are ignored.
	 */
	if (version_flag) {
		fprintf(stderr, "%s\n", SW_VERSION);
		exit(EXIT_SUCCESS);
		/* NOTREACHED */
	}

	/*
	 * If the verbosity level has not yet been set, then use the default level,
	 * INFO.
	 */
	if (!verbosity_flag) {
		verbose_level_set(INFO);
	}

	if (config->ptu_file != NULL && config->ptu_file[0] == '-') {
		output(FATAL,
			"\n"
			"The --update option requires an argument but the\n"
			"next CLI input value was \"%s\" which starts with a\n"
			"dash, suggesting that this a separate CLI option.\n",
			config->ptu_file);
		abort();
		/* NOTREACHED */
	}

	if (!config->check_active && !config->check_target && !config->update) {
		_print_help();
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}
}

static void _print_help()
{
	fprintf(stderr,
"ptupdater %s, a Parade Technologies Touch Firmware Updater tool\n",
		SW_VERSION);
	fprintf(stderr,
"Usage: ptupdater <hidraw/sysfs/node/filepath> [options]\n\n");
	fprintf(stderr,
"Overview:\n"
"  Parade Technologies Touch Firmware Updater command line tool that checks\n"
"  and/or updates the firmware that is running on the Parade touch processor.\n"
"  Firmware updates are embedded in a Parade Touch Updater (PTU) file that\n"
"  is provided via the '--check-target' or '--update' parameters. The PTU\n"
"  file is in XML format. PTU files are expected to only ever be constructed\n"
"  and provided by Parade Technologies so report any issues to\n"
"  support@paradetech.com. If you are internal to Parade, and are trying to\n"
"  construct a valid PTU file, then please refer to the PTU wiki page,\n"
"  including the \"PtUpdater Specifics\" section.\n"
"\n"
"Options:\n"
"  -h,  --help                   Prints this message.\n"
"\n"
"       --check-active           Check and display the active firmware\n"
"                                version running on the touch processor.\n"
"       --check-target FILEPATH  Check and display the target firmware\n"
"                                version by parsing the header of the binary\n"
"                                image embedded in the PTU file.\n"
"\n"
"       --i2c-bus      I2C-BUS   The I2C bus of the Parade touch device,\n"
"                                which is required for using PIP2\n"
"                                ROM-Bootloader interface. Therefore, if this\n"
"                                argument is not provided, then the Secondary\n"
"                                Loader Image will certainly not be updated.\n"
"\n"
"       --update       FILEPATH  Check the active firmware version running on\n"
"                                the touch processor, and update it if it\n"
"                                does not match the target firmware version.\n"
"\n"
"       --verbose      LEVEL     Changes the verboseness level of the tool.\n"
"                                value is 5 (INFO). Verboseness levels are\n"
"                                cumulative. Therefore, setting a level of 2\n"
"                                will show FATAL and RESULT output\n"
"                                  0: QUIET  No output.\n"
"                                  1: FATAL  Unrecoverable. Execution ends.\n"
"                                  2: RESULT Not used.\n"
"                                  3: ERROR  Serious issue, but, recoverable.\n"
"                                  4: WARN   Information that may affect test\n"
"                                            test results but are acceptable\n"
"                                            to this tool\n"
"                                  5: INFO   Information that may be useful\n"
"                                            to the user\n"
"                                  6: DEBUG  Information that is never needed\n"
"                                            in a working system and may be\n"
"                                            helpful for debug of a system\n"
"                                            that is not working properly.\n"
"\n"
"       --version                Prints the ptupdater tool version number and\n"
"                                then exits.\n"
"\n"
"Examples:\n"
"\n"
"  ptupdater /dev/hidraw0 --update /lib/firmware/parade.ptu\n"
"\n"
"  ptupdater /dev/hidraw0 --check-active\n"
"\n"
"  ptupdater /dev/hidraw0 --check-target /lib/firmware/parade.ptu\n"
"\n"
"\n"
	);
}

static int _run(const PtUpdater_Config* config)
{
	output(DEBUG, "%s: Starting.\n", __func__);
	int rc = EXIT_FAILURE;

	if (config->check_active) {
		FW_Version active_version;
		if (EXIT_SUCCESS != get_fw_version_from_flash(&active_version)) {
			rc = EXIT_FAILURE;
			goto END;
		}
		output(INFO, "Active Version: %d.%d.%d.%d\n",
			active_version.major,
			active_version.minor,
			active_version.rev_control,
			active_version.config_ver);
	}

	// if (config->check_target
	// 		&& EXIT_SUCCESS != process_ptu_file(config->ptu_file, false)) {
	// 	rc = EXIT_FAILURE;
	// 	goto END;
	// }

	// if (config->update
	// 		&& EXIT_SUCCESS != process_ptu_file(config->ptu_file, true)) {
	// 	rc = EXIT_FAILURE;
	// 	goto END;
	// }

	rc = EXIT_SUCCESS;

END:
	if (EXIT_SUCCESS != teardown_pip2_api()
			|| EXIT_SUCCESS != teardown_pip3_api()) {
		rc = EXIT_FAILURE;
	}
	return rc;
}

static int _setup(const PtUpdater_Config* config)
{
	output(DEBUG, "%s: Starting.\n", __func__);

	if (EXIT_SUCCESS != init_hidraw_api(config->hidraw_sysfs_node_file)) {
		return EXIT_FAILURE;
		/* NOTREACHED */
	}

	if (config->use_i2c_dev) {
		if (EXIT_SUCCESS != setup_pip2_api(CHANNEL_TYPE_I2CDEV, config->i2c_bus,
				config->i2c_addr)) {
			return EXIT_FAILURE;
			/* NOTREACHED */
		}
	} else {
		output(DEBUG,
			"PIP2 interface will not be used because the I2C bus was not "
			"specified.\n");
	}

	if (EXIT_SUCCESS != setup_pip3_api(&hidraw_channel,
			HID_REPORT_ID_SOLICITED_RESPONSE)) {
		if (is_pip2_api_active()) {
			output(WARNING,
	"The HIDRAW driver/interface is unavailable/unresponsive, which\n"
	"\tsuggests that the Parade touch device is stuck in the\n"
	"\tROM-Bootloader and/or both the Primary and Seconary images have\n"
	"\tbeen erased/corrupted. PtUpdater can/will use the PIP2 ROM-BL\n"
	"\tinterface to update all of the touch device's firmware, however,\n"
	"\tthe host device will need to be restarted to allow the HIDRAW\n"
	"\tdriver to correctly enumerate.\n");
		} else {
			output(ERROR,
	"%s: The Parade touch device's firmware cannot be updated via the\n"
	"\tHIDRAW driver/interface, because it is unavailable/unresponsive\n"
	"\tThis suggests that the touch device is stuck in the ROM-Bootloader\n"
	"\tand/or both the Primary and Seconary images have been\n"
	"\terased/corrupted.\n",
				__func__);
			return EXIT_FAILURE;
			/* NOTREACHED */
		}
	}

	return EXIT_SUCCESS;
}
