/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_ENCODING_H
#define MGC_ENCODING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"

bool encoding_utf8_get_next_char(const char *utf8_str, const char **next);
uint32_t encoding_utf8_to_unicode(const char *utf8_str, const char **next);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_ENCODING_H*/
