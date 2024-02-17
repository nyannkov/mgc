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

enum mgc_sub_screen_ctrl_req {
    MGC_SUB_SCREEN_CTRL_REQ_NONE = 0,
    MGC_SUB_SCREEN_CTRL_REQ_SHOW_TEXT_WAIT_CURSOR_END,
    MGC_SUB_SCREEN_CTRL_REQ_SHOW_TEXT_WAIT_BUTTON_PRESSED,
    MGC_SUB_SCREEN_CTRL_REQ_SHOW_TEXT_WAIT_RECEIVE_YESNO,
    MGC_SUB_SCREEN_CTRL_REQ_CLOSE,
};

enum mgc_sub_screen_ctrl_state {
    MGC_SUB_SCREEN_CTRL_00_STOP = 0,
    MGC_SUB_SCREEN_CTRL_10_WAIT_CURSOR_END,
    MGC_SUB_SCREEN_CTRL_11_FINISHED,
    MGC_SUB_SCREEN_CTRL_20_WAIT_CURSOR_END,
    MGC_SUB_SCREEN_CTRL_21_WAIT_BUTTON_PRESSED,
    MGC_SUB_SCREEN_CTRL_22_FINISHED,
    MGC_SUB_SCREEN_CTRL_30_WAIT_CURSOR_END,
    MGC_SUB_SCREEN_CTRL_31_WAIT_RESPONSE,
    MGC_SUB_SCREEN_CTRL_32_FINISHED_RECEIVED_YES,
    MGC_SUB_SCREEN_CTRL_33_FINISHED_RECEIVED_NO,
};

void sub_screen_local_set_state(enum mgc_sub_screen_ctrl_state state);
enum mgc_sub_screen_ctrl_state sub_screen_local_get_state(void);
bool sub_screen_local_check_request(enum mgc_sub_screen_ctrl_req *out);
const char *sub_screen_local_get_req_text(void);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_SUB_SCREEN_LOCAL_H*/
