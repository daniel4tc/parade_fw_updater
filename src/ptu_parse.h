/*
 * Copyright (c) Parade Technologies, Ltd. 2023.
 */
#ifndef _XML_H
#define _XML_H

#include <libxml/parser.h>
#include <libxml/xmlschemastypes.h>
#include <openssl/md5.h>
#include "base64.h"
#include "fw_version.h"

extern int process_ptu_file(const char* file, bool update_fw);

#endif /* _XML_H */

