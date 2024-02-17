/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "encoding.h"

bool encoding_utf8_get_next_char(const char *utf8_str, const char **next) {
    char ch;
    if ( ( utf8_str == NULL ) ||
         ( next == NULL )
    ) {
        MGC_WARN("Invalid parameters");
        return false;
    }
    ch = utf8_str[0];
    if ((ch & 0x80) == 0x00) {
        *next = &utf8_str[1];
    } else if ((ch & 0xE0) == 0xC0) {
        *next = &utf8_str[2];
    } else if ((ch & 0xF0) == 0xE0) {
        *next = &utf8_str[3];
    } else if ((ch & 0xF8) == 0xF0) {
        *next = &utf8_str[4];
    } else {
        MGC_WARN("character isn't in UTF-8");
        *next = &utf8_str[0];
        return false;
    }
    return true;
}

uint32_t encoding_utf8_to_unicode(const char *utf8_str, const char **next) {
    uint32_t unicode;
    char ch;
    size_t seq_len;

    if ( next != NULL ) {
        *next = utf8_str;
    }
    if (utf8_str == NULL ) {
        return 0;
    }

    ch = utf8_str[0];
    if ((ch & 0x80) == 0x00) {
        unicode = ch;
        seq_len = 1;
    } else if ((ch & 0xE0) == 0xC0) {
        unicode = ch & 0x1F;
        seq_len = 2;
    } else if ((ch & 0xF0) == 0xE0) {
        unicode = ch & 0x0F;
        seq_len = 3;
    } else if ((ch & 0xF8) == 0xF0) {
        unicode = ch & 0x07;
        seq_len = 4;
    } else {
        return 0;
    }

    for (size_t i = 1; i < seq_len; i++) {
        if ( (utf8_str[i] & 0xC0) != 0x80 ) {
            return 0;
        }
        unicode = (unicode << 6) | (utf8_str[i] & 0x3F);
    }

    if ( next != NULL ) {
        *next = &utf8_str[seq_len];
    }
    return unicode;
}
