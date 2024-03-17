/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */

#include "sub_screen_local.h"
#include "dlgflow_handler_sub_screen.h"

struct request_counter {
    uint8_t show_dialoguebox;
    uint8_t close_dialoguebox;
    uint8_t show_selectbox;
    uint8_t close_selectbox;
};

struct response_counter {
    uint8_t proc_dialoguebox;
    uint8_t proc_selectbox;
};

static struct request_counter req_counter, prev_req_counter;
static struct response_counter resp_counter, prev_resp_counter;

static const char *show_dialoguebox_text;
static uint32_t show_dialoguebox_flags;
static int32_t proc_dialoguebox_result;

static const char **show_selectbox_item_array;
static size_t show_selectbox_item_count;
static uint32_t show_selectbox_flags;
static int32_t proc_selectbox_result;

static void show_dialoguebox(const char *text, uint32_t flags);
static bool proc_dialoguebox(int32_t *result);
static void close_dialoguebox(void);
static void show_selectbox(const char **item_array, size_t item_count, uint32_t flags);
static bool proc_selectbox(int32_t *result);
static void close_selectbox(void);

const mgc_dlgflow_if_t dlgflow_handler_sub_screen = {
    .show_dialoguebox = show_dialoguebox,
    .proc_dialoguebox = proc_dialoguebox,
    .close_dialoguebox = close_dialoguebox,
    .show_selectbox = show_selectbox,
    .proc_selectbox = proc_selectbox,
    .close_selectbox = close_selectbox
};

static void show_dialoguebox(const char *text, uint32_t flags) {
    show_dialoguebox_text = text;
    show_dialoguebox_flags = flags;
    req_counter.show_dialoguebox++;
}

static bool proc_dialoguebox(int32_t *result) {
    if ( prev_resp_counter.proc_dialoguebox != resp_counter.proc_dialoguebox ) {
        prev_resp_counter.proc_dialoguebox = resp_counter.proc_dialoguebox;
        *result = proc_dialoguebox_result;
        return true;
    }
    return false;
}

static void close_dialoguebox(void) {
    req_counter.close_dialoguebox++;
}

static void show_selectbox(const char **item_array, size_t item_count, uint32_t flags) {
    show_selectbox_item_array = item_array;
    show_selectbox_item_count = item_count;
    show_selectbox_flags = flags;
    req_counter.show_selectbox++;
}

static bool proc_selectbox(int32_t *result) {
    if ( prev_resp_counter.proc_selectbox != resp_counter.proc_selectbox ) {
        prev_resp_counter.proc_selectbox = resp_counter.proc_selectbox;
        *result = proc_selectbox_result;
        return true;
    }
    return false;
}

static void close_selectbox(void) {
    req_counter.close_selectbox++;
}

void init_dlgflow_handler(void) {
    for ( size_t i = 0; i < sizeof(struct request_counter); i++ ) {
        ((uint8_t *)(&req_counter))[i] = 0;
        ((uint8_t *)(&prev_req_counter))[i] = 0;
    }
    for ( size_t i = 0; i < sizeof(struct response_counter); i++ ) {
        ((uint8_t *)(&resp_counter))[i] = 0;
        ((uint8_t *)(&prev_resp_counter))[i] = 0;
    }
    show_dialoguebox_text = NULL;
    show_dialoguebox_flags = 0;
    proc_dialoguebox_result = 0;

    show_selectbox_item_array = NULL;
    show_selectbox_item_count = 0;
    show_selectbox_flags = 0;
    proc_selectbox_result = 0;
}

void set_proc_dialoguebox_result(int32_t result) {
    proc_dialoguebox_result = result;
    resp_counter.proc_dialoguebox++;
}

void set_proc_selectbox_result(int32_t result) {
    proc_selectbox_result = result;
    resp_counter.proc_selectbox++;
}

const char *get_show_dialoguebox_text(void) {
    return show_dialoguebox_text;
}

uint32_t get_show_dialoguebox_flags(void) {
    return show_dialoguebox_flags;
}

const char **get_show_selectbox_item_array(void) {
    return show_selectbox_item_array;
}

size_t get_show_selectbox_item_count(void) {
    return show_selectbox_item_count;
}

uint32_t get_show_selectbox_flags(void) {
    return show_selectbox_flags;
}

bool check_request_show_dialoguebox(void) {
    if ( prev_req_counter.show_dialoguebox != req_counter.show_dialoguebox ) {
        prev_req_counter.show_dialoguebox = req_counter.show_dialoguebox;
        return true;
    } else {
        return false;
    }
}

bool check_request_close_dialoguebox(void) {
    if ( prev_req_counter.close_dialoguebox != req_counter.close_dialoguebox ) {
        prev_req_counter.close_dialoguebox = req_counter.close_dialoguebox;
        return true;
    } else {
        return false;
    }
}

bool check_request_show_selectbox(void) {
    if ( prev_req_counter.show_selectbox != req_counter.show_selectbox ) {
        prev_req_counter.show_selectbox = req_counter.show_selectbox;
        return true;
    } else {
        return false;
    }
}

bool check_request_close_selectbox(void) {
    if ( prev_req_counter.close_selectbox != req_counter.close_selectbox ) {
        prev_req_counter.close_selectbox = req_counter.close_selectbox;
        return true;
    } else {
        return false;
    }
}
