/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "mgc/sequencer/dlghdr_if.h"
#include "sub_screen_local.h"

#define MAX_COUNT_VALUE     60000

static uint16_t req_counter;
static uint16_t prev_req_counter;
static const char *req_text;
static enum mgc_sub_screen_ctrl_req curr_req;
static enum mgc_sub_screen_ctrl_state curr_state;

static void init(void) {
    req_counter = 0;
    prev_req_counter = 0;
    curr_req = MGC_SUB_SCREEN_CTRL_REQ_NONE;
    req_text = "";
}

static void display_text(const char * text, enum mgc_dlg_wait_type wait_type) {
    if ( req_counter < MAX_COUNT_VALUE ) {
        req_counter++;
    } else {
        req_counter = 1;
    }
    req_text = text;
    switch (wait_type) {
    case MGC_DLG_ON_CURSOR_END:
        curr_req = MGC_SUB_SCREEN_CTRL_REQ_SHOW_TEXT_WAIT_CURSOR_END;
        break;
    case MGC_DLG_ON_BUTTON_PRESSED:
        curr_req = MGC_SUB_SCREEN_CTRL_REQ_SHOW_TEXT_WAIT_BUTTON_PRESSED;
        break;
    case MGC_DLG_ON_RECEIVE_YESNO:
        curr_req = MGC_SUB_SCREEN_CTRL_REQ_SHOW_TEXT_WAIT_RECEIVE_YESNO;
        break;
    default:
        curr_req = MGC_SUB_SCREEN_CTRL_REQ_SHOW_TEXT_WAIT_BUTTON_PRESSED;
        break;
    }
}

static void close_text(void) {
    if ( req_counter < MAX_COUNT_VALUE ) {
        req_counter++;
    } else {
        req_counter = 1;
    }
    curr_req = MGC_SUB_SCREEN_CTRL_REQ_CLOSE;
}

bool is_display_text_finished(void) {
    switch (curr_state) {
    case MGC_SUB_SCREEN_CTRL_11_FINISHED:/*FALLTHROUGH*/
    case MGC_SUB_SCREEN_CTRL_22_FINISHED:/*FALLTHROUGH*/
    case MGC_SUB_SCREEN_CTRL_32_FINISHED_RECEIVED_YES:/*FALLTHROUGH*/
    case MGC_SUB_SCREEN_CTRL_33_FINISHED_RECEIVED_NO:
        return true;
    default:
        return false;
    }
}

bool is_response_yes(void) {
    if ( curr_state == MGC_SUB_SCREEN_CTRL_32_FINISHED_RECEIVED_YES ) {
        return true;
    } else {
        return false;
    }
}

const mgc_dlghdr_if_t dlghdr_sub_screen = {
    .init = init,
    .display_text = display_text,
    .close_text = close_text,
    .is_display_text_finished = is_display_text_finished,
    .is_response_yes = is_response_yes,
};

void sub_screen_local_set_state(enum mgc_sub_screen_ctrl_state state) {
    curr_state = state;
}

bool sub_screen_local_check_request(enum mgc_sub_screen_ctrl_req *out) {
    bool update_flag = false;
    if ( prev_req_counter != req_counter ) {
        prev_req_counter = req_counter;
        update_flag = true;
    } else {
        update_flag = false;
    }
    if ( out != NULL ) {
        *out = curr_req;
    }
    return update_flag;
}

const char *sub_screen_local_get_req_text(void) {
    return req_text;
}

enum mgc_sub_screen_ctrl_state sub_screen_local_get_state(void) {
    return curr_state;
}

