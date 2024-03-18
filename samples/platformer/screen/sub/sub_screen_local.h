/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_SUB_SCREEN_LOCAL_H
#define MGC_SUB_SCREEN_LOCAL_H

#include "mgc/mgc.h"

#ifdef __cplusplus
extern "C" {
#endif

void init_dlgflow_handler(void);
void set_proc_dialoguebox_result(int32_t result);
void set_proc_selectbox_result(int32_t result);

const char *get_show_dialoguebox_text(void);
uint32_t get_show_dialoguebox_flags(void);
const char **get_show_selectbox_item_array(void);
size_t get_show_selectbox_item_count(void);
uint32_t get_show_selectbox_flags(void);

bool check_request_show_dialoguebox(void);
bool check_request_close_dialoguebox(void);
bool check_request_show_selectbox(void);
bool check_request_close_selectbox(void);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_SUB_SCREEN_LOCAL_H*/
