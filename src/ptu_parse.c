/*
 * Copyright (c) Parade Technologies, Ltd. 2023.
 */
#include "ptu_1.h"
#include "ptu_parse.h"

#define PTU_SCHEMA_1 1
#define MD5_CHECKSUM_STRLEN MD5_DIGEST_LENGTH * 2 + 1
#define MAX_ERASE_FLASH_FILES 10

static int _xml_get_elements_erase(const xmlNodePtr, ByteData* files);
static void _xml_get_prop(const xmlNode* node, char *property, char **value);
static int _xml_get_prop_image(const xmlNode* node, ByteData* image);
static int _xml_get_prop_int(const xmlNode* node, char *property);
static bool _xml_has_primary_image(const xmlNodePtr node);
static xmlAttrPtr _xml_has_prop(const xmlNode *node, const char *name);
static int _xml_process_element_array_files(const xmlNodePtr node,
		bool update_fw);
static int _xml_process_element_file(const xmlNodePtr node, bool update_fw,
		bool* done);
static int _xml_show_host_interface_pid(const xmlNodePtr node);
static int _xml_strcmp(const xmlChar *str1, const char *str2);
static int _xml_validate_ptu_file(int schema_version, const char* file);

int process_ptu_file(const char* file, bool update_fw)
{
	output(DEBUG, "%s: Starting.\n", __func__);

	const xmlDoc* xml_doc = xmlReadFile(file, NULL, 0);
	if (xml_doc == NULL) {
		output(FATAL, "Could not read the %s file.\n", file);
		return EXIT_FAILURE;
		/* NOTREACHED */
	}
	xmlNodePtr xml_root_node = xmlDocGetRootElement(xml_doc);
	if (xml_root_node == NULL) {
		output(FATAL, "Could not find the root element of the PTU file.\n");
		return EXIT_FAILURE;
		/* NOTREACHED */
	} else if (_xml_strcmp(xml_root_node->name, "ptu") != 0) {
		output(FATAL,
			"The root element of the PTU file must be named 'ptu'.\n");
		return EXIT_FAILURE;
		/* NOTREACHED */
	}

	if (NULL == _xml_has_prop(xml_root_node, "schema_version")) {
		output(FATAL,
			"PTU file is missing the required 'schema_version' attribute.\n");
		return EXIT_FAILURE;
		/* NOTREACHED */
	}

	int ptu_schema_version = _xml_get_prop_int(xml_root_node, "schema_version");
	if (_xml_validate_ptu_file(ptu_schema_version, file) != EXIT_SUCCESS) {
		return EXIT_FAILURE;
		/* NOTREACHED */
	}

	for (xmlNodePtr cur_node = xml_root_node->children; cur_node;
			cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			output(DEBUG, "node: '%s'\n", cur_node->name);

			if (_xml_strcmp(cur_node->name, "bootload_flow") == 0
					&& EXIT_SUCCESS
						!= _xml_process_element_array_files(cur_node->children,
								update_fw)) {
				return EXIT_FAILURE;
				/* NOTREACHED */
			} else if (_xml_strcmp(cur_node->name, "global_config") == 0
					&& EXIT_SUCCESS
						!= _xml_show_host_interface_pid(cur_node->children)) {
				return EXIT_FAILURE;
				/* NOTREACHED */
			}
		}
	}

	return EXIT_SUCCESS;
}

static int _xml_get_elements_erase(const xmlNodePtr node, ByteData* files)
{
	for (xmlNodePtr cur_node = node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE
				&& _xml_strcmp(cur_node->name, "erase") == 0) {
			if (files->len > MAX_ERASE_FLASH_FILES) {
				output(ERROR,
					"%s: This tool does not support erasing more than %d flash "
					"files before updating the target image.\n",
					__func__, MAX_ERASE_FLASH_FILES);
				return EXIT_FAILURE;
				/* NOTREACHED */
			}

			/*
			 * The PTU-XML field 'file_id' is restricted to 1-99 so casting to
			 * a 'uint8_t' is never expected to cause overflow.
			 */
			files->data[files->len] =
				(uint8_t) _xml_get_prop_int(cur_node, "file_id");
			files->len++;
		}
	}
	return EXIT_SUCCESS;
}

static void _xml_get_prop(const xmlNode* node, char *property, char **value)
{
	*value = (char *) xmlGetProp(node, (const xmlChar *) property);
	if (NULL == *value) {
		output(WARNING, "Failed to get value for XML property: %s\n", property);
	}
}

static int _xml_get_prop_image(const xmlNode* node, ByteData* image)
{
	output(DEBUG, "%s: Starting.\n", __func__);
	int rc = EXIT_FAILURE;
	char* b64_str = NULL;
	uint8_t calculated_md5sum_bytes[MD5_DIGEST_LENGTH];
	char calculated_md5sum_str[MD5_CHECKSUM_STRLEN];
	char expected_md5sum_str[MD5_CHECKSUM_STRLEN];
	MD5_CTX mdContext;

	if (node == NULL || image == NULL) {
		output(ERROR, "%s: NULL argument given.\n", __func__);
		return EXIT_FAILURE;
		/* NOTREACHED */
	} else if (image->data != NULL) {
		output(ERROR,
				"%s: Argument member image->data must initially be NULL.\n",
				__func__);
		return EXIT_FAILURE;
		/* NOTREACHED */
	}

	_xml_get_prop(node, "image", &b64_str);
	if (NULL == b64_str) {
		rc = EXIT_FAILURE;
		output(ERROR,
			"%s: This XML element does not have an 'image' attribute.\n",
			__func__);
		goto END;
	}

	char* temp_md5sum_str = NULL;
	_xml_get_prop(node, "md5sum", &temp_md5sum_str);
	if (NULL == temp_md5sum_str) {
		rc = EXIT_FAILURE;
		output(ERROR,
			"%s: The XML element does not have an 'md5sum' attribute.\n",
			__func__);
		goto END;
	}
	strcpy(expected_md5sum_str, temp_md5sum_str);
	expected_md5sum_str[MD5_CHECKSUM_STRLEN - 1] = '\0';
	free(temp_md5sum_str);

	for (int i = 0; expected_md5sum_str[i] != '\0'; i++) {
		expected_md5sum_str[i] = (char) toupper(expected_md5sum_str[i]);
	}

	if (b64_decode(b64_str, image) != EXIT_SUCCESS) {
		rc = EXIT_FAILURE;
		goto END;
	}

	output(DEBUG, "Calculating the MD5 checksum of the utility firmware.\n");
	MD5_Init(&mdContext);
	MD5_Update(&mdContext, image->data, image->len);
	MD5_Final(calculated_md5sum_bytes, &mdContext);

	output(DEBUG,
			"Verifying that the calculated and expected MD5 checksum values "
			"match.\n");
	for(int i = 0; i < MD5_DIGEST_LENGTH; i++) {
		snprintf(&(calculated_md5sum_str[i * 2]), 3, "%02X",
				calculated_md5sum_bytes[i]);
	}
	calculated_md5sum_str[MD5_CHECKSUM_STRLEN - 1] = '\0';

	if (strcmp(expected_md5sum_str, calculated_md5sum_str) != 0) {
		rc = EXIT_FAILURE;
		output(DEBUG,
				"%s: MD5 checksum mismatch.\n"
				"\tExpected:   '%s'\n"
				"\tCalculated: '%s'\n",
				__func__, expected_md5sum_str,
				calculated_md5sum_str);
	} else {
		rc = EXIT_SUCCESS;
	}

END:
	free(b64_str);
	output(DEBUG, "%s: Returning.\n", __func__);
	return rc;
}

static int _xml_get_prop_int(const xmlNode* node, char *property)
{
	char *tmp = NULL;
	int value;

	_xml_get_prop(node, property, &tmp);
	if (NULL != tmp) {
		value = atoi(tmp);
		xmlFree(tmp);
		errno = 0;
		return value;
		/* NOTREACHED */
	}
	output(DEBUG, "%s: property: %s, int: NULL\n", __func__, property);
	errno = 1;
	return 0;
}

static bool _xml_has_primary_image(const xmlNodePtr node)
{
	output(DEBUG, "%s: Starting.\n", __func__);
	int count = 0;
	for (xmlNodePtr cur_node = node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE
				&& _xml_strcmp(cur_node->name, "file") == 0
				&& _xml_has_prop(cur_node, "file_type")) {
			char* file_type = NULL;
			char* file_format = NULL;
			_xml_get_prop(cur_node, "file_type", &file_type);
			_xml_get_prop(cur_node, "file_format", &file_format);
			if (_xml_strcmp(file_format, "bin") == 0
						&& _xml_strcmp(file_type, "primary_image") == 0) {
				count++;
			}
		}
	}

	return count == 1;
}

static xmlAttrPtr _xml_has_prop(const xmlNode *node, const char *name)
{
	return xmlHasProp(node, (const xmlChar *) name);
}

static int _xml_process_element_array_files(const xmlNodePtr node,
		bool update_fw)
{
	output(DEBUG, "%s: Starting.\n", __func__);
	int rc = EXIT_SUCCESS;
	bool done = false;

	if (!_xml_has_primary_image(node)) {
		output(FATAL, "The PTU must contain exactly one 'file' element with "
			"attributes 'file_type'=\"primary_image\" and "
			"'file_format'=\"bin\".\n");
		return EXIT_FAILURE;
		/* NOTREACHED */
	}

	for (xmlNodePtr cur_node = node;
			cur_node && rc == EXIT_SUCCESS && !done;
			cur_node = cur_node->next) {

		if (cur_node->type == XML_ELEMENT_NODE
				&& _xml_strcmp(cur_node->name, "file") == 0) {

			rc = _xml_process_element_file(cur_node, update_fw, &done);
		}
	}

	return rc;
}

static int _xml_process_element_file(const xmlNodePtr node, bool update_fw,
		bool* done)
{
	output(DEBUG, "%s: Starting.\n", __func__);
	int rc = EXIT_SUCCESS;

	char* display_name = NULL;
	/*
	 * The PTU-XML field 'file_id' is restricted to 1-99 so casting to a
	 * 'uint8_t' is never expected to cause overflow.
	 */
	uint8_t file_id = (uint8_t) _xml_get_prop_int(node, "file_id");
	char* file_type = NULL;
	char* file_format = NULL;
	ByteData image = { .data = NULL, .len = 0 };
	uint8_t flash_files_to_erase_id_list[MAX_ERASE_FLASH_FILES];
	ByteData flash_files_to_erase = {
		.data = flash_files_to_erase_id_list,
		.len = 0
	};

	_xml_get_prop(node, "display_name", &display_name);
	if (_xml_has_prop(node, "file_type")) {
		_xml_get_prop(node, "file_type", &file_type);
	}

	_xml_get_prop(node, "file_format", &file_format);

	if (EXIT_SUCCESS != _xml_get_prop_image(node, &image)) {
		rc = EXIT_FAILURE;
		goto FREE_MEM;
	}

	if (EXIT_SUCCESS != _xml_get_elements_erase(node->children,
			&flash_files_to_erase)) {
		rc = EXIT_FAILURE;
		goto FREE_MEM;
	}

	if (
			   file_format != NULL
			&& _xml_strcmp(file_format, "bin") == 0
			&& file_type != NULL
			&& strcmp(file_type, "primary_image") == 0
			) {
		const FW_Bin_Header* bin_header = (const FW_Bin_Header*) image.data;
		FW_Version target_version;

		output(DEBUG,
			"Touch firmware/Primary binary image provided as flash "
			"file ID %d (%s).\n",
			file_id, display_name);
		if (EXIT_SUCCESS
				!= get_fw_version_from_bin_header(bin_header,
						&target_version)) {
			rc = EXIT_FAILURE;
			goto FREE_MEM;
		}

		if (!update_fw) {
			/* 
			 * If we are not updating the firmware, then we are just trying to
			 * get the version of the primary touch firmware image that is
			 * embedded in the PTU file.
			 */
			output(INFO, "Target Version: %d.%d.%d.%d\n",
					target_version.major,
					target_version.minor,
					target_version.rev_control,
					target_version.config_ver);
			*done = true;
			rc = EXIT_SUCCESS;
			goto FREE_MEM;
		}
	}

	if (update_fw) {
		/*
		 * Determine whether the PIP2 ROM-BL interface must be used
		 * directly in order to update the an image, or if we can simply
		 * use the Secondary Loader image.
		 */
		Flash_Loader_Options loader_options; 
		if (file_type != NULL
				&& _xml_strcmp(file_type, "secondary_image") == 0) {
			loader_options.list[0] = FLASH_LOADER_PIP2_ROM_BL;
			loader_options.list[1] = FLASH_LOADER_NONE;
			output(DEBUG,
	"The PTU file contains a Secondary Loader image. Updating this will\n"
	"\trequire using the PIP2 ROM-BL.\n"
				);
		} else {
			loader_options.list[0] = FLASH_LOADER_SECONDARY_IMAGE;
			loader_options.list[1] = FLASH_LOADER_PIP2_ROM_BL;
			loader_options.list[2] = FLASH_LOADER_NONE;
			output(DEBUG,
	"Will first try using the Secondary Loader image to update the %s image\n"
	"\tbut if that doesn't work, will try the PIP2 ROM-BL too.\n",
					display_name);
		}
		
		if (EXIT_SUCCESS
				!= write_image_to_dut_flash_file(
						file_id,
						&image,
						&flash_files_to_erase,
						&loader_options)) {
			rc = EXIT_FAILURE;
		}
	}

FREE_MEM:
	free(display_name);
	free(file_type);
	free(file_format);
	free(image.data);

	return rc;
}

static int _xml_show_host_interface_pid(const xmlNodePtr node)
{
	output(DEBUG, "%s: Starting.\n", __func__);

	for (xmlNodePtr cur_node = node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE
				&& _xml_strcmp(cur_node->name, "host_interface") == 0
				&& _xml_has_prop(cur_node, "pid")) {
			char* pid = NULL;
			_xml_get_prop(cur_node, "pid", &pid);
			output(INFO, "Target PID: %s\n", pid);
			free(pid);
			return EXIT_SUCCESS;
			/* NOTREACHED */
		}
	}

	output(FATAL, "The PTU is missing the host_interface 'pid' attribute.\n");
	return EXIT_FAILURE;
}

static int _xml_strcmp(const xmlChar *str1, const char *str2)
{
	return xmlStrcmp(str1, (const xmlChar *) str2);
}

static int _xml_validate_ptu_file(int schema_version, const char* file)
{
	xmlDocPtr doc;
	xmlSchemaPtr schema = NULL;
	xmlSchemaParserCtxtPtr ctxt = NULL;

	xmlLineNumbersDefault(1);

	switch (schema_version) { /* NOSONAR (c:S1301) */
	/*
	 * There's currently only one supported PTU schema (XSD) version, 1.0.
	 * But eventually there be more, so a switch statement makes sense here
	 * despite SonarLint's complaint, hence the "NOSONAR".
	 */
	case PTU_SCHEMA_1:
		ctxt = xmlSchemaNewMemParserCtxt((char *) ptu_1, ptu_1_len);
		break;
	default:
		output(FATAL,"%s: Unsupported schema version given: %d\n",
				__func__, schema_version);
		goto END;
	}

	xmlSchemaSetParserErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf,
		(xmlSchemaValidityWarningFunc) fprintf, stderr);
	schema = xmlSchemaParse(ctxt);
	xmlSchemaFreeParserCtxt(ctxt);

	doc = xmlReadFile(file, NULL, 0);
	if (doc == NULL) {
		output(FATAL, "Could not parse %s\n", file);
	} else {
		int ret;

		ctxt = xmlSchemaNewValidCtxt(schema);
		xmlSchemaSetValidErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf,
			(xmlSchemaValidityWarningFunc) fprintf, stderr);
		ret = xmlSchemaValidateDoc(ctxt, doc);
		if (ret == 0) {
			output(DEBUG, "Schema version %d validates.\n", schema_version);
		} else if (ret > 0) {
			output(FATAL, "Schema version %d fails to validate.\n",
				schema_version);
			if (verbose_level_get() >= DEBUG) {
				xmlSchemaDump(stdout, schema);
			}
			xmlSchemaFreeValidCtxt(ctxt);
			xmlFreeDoc(doc);
			return EXIT_FAILURE;
		} else {
			output(FATAL,
				"Schema version %d validation generated an internal error.\n",
				schema_version);
		}
		xmlSchemaFreeValidCtxt(ctxt);
		xmlFreeDoc(doc);
	}

END:
	if (schema != NULL) {
		xmlSchemaFree(schema);
	}

	xmlSchemaCleanupTypes();
	xmlCleanupParser();
	xmlMemoryDump();

	return EXIT_SUCCESS;
}
