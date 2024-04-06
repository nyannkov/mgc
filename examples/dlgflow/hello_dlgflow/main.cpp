/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include <stdio.h>
#include "mgc/mgc.h"
#include "system/system.h"

static const char * item_array_yesno[2] = { "Yes", "No" };
static const mgc_dlg_items_t items_yesno = { item_array_yesno, countof(item_array_yesno) };

enum purchase_decision {
    BUY_APPLE = 0,
    BUY_ORANGE,
    BUY_BANANA,
    DO_NOT_BUY,
};
static const char * item_array_purchase_decision_options[DO_NOT_BUY+1] = { "Apples", "Oranges", "Bananas", "Not buy" };
static const mgc_dlg_items_t items_purchase_decision_options = {
    item_array_purchase_decision_options,
    countof(item_array_purchase_decision_options)
};

static char text_buffer[256];
static bool auto_switch_flag;

/*
  Define the conversation flow as an array of nodes.
  Assign a unique ID to each node.
  Specify the type of each node using 'type'.
  If a node's type is set to MGC_DLG_TYPE_TEXT, it will display the text stored in the 'text' field of the params union.
  If the type is MGC_DLG_TYPE_SELECT, it will present the options stored in the 'items' field of the params union.
  After processing a node, execute the function registered in 'cb_before_switch_node' to determine the ID of the next node to execute.
  If no function is registered, the next node in sequence will be selected. 'flags' are general-purpose flags that users can utilize freely.
  In this example, they are utilized to achieve simultaneous display of text and options.
  Set 'terminal' to true for nodes that are meant to terminate the flow.
  The ID of the terminal node is intended to be used as a condition for triggering events and altering processing.
*/
static const mgc_dlgnode_t node_array[] = {
    { .id=0x00, .type=MGC_DLG_TYPE_TEXT, .params={.text="Welcome. Would you like too buy something?" }, .flags=0x01 },
    { .id=0x10, .type=MGC_DLG_TYPE_SELECT, .params={ .items = &items_yesno },
        .cb_before_switch_node = [](const mgc_dlgflow_t *dlgflow) -> mgc_node_id_t { return ( dlgflow->result == 0 ) ? 0x20 : 0xF0; }
    },
    { .id=0x20, .type=MGC_DLG_TYPE_TEXT, .params={.text="What would you like to buy?" }, .flags=0x01 },
    { .id=0x21, .type=MGC_DLG_TYPE_SELECT, .params= { .items = &items_purchase_decision_options },
        .cb_before_switch_node = [](const mgc_dlgflow_t *dlgflow) -> mgc_node_id_t {
            int price;
            switch ((enum purchase_decision)dlgflow->result) {
            case BUY_APPLE:
                price = 500;
                break;
            case BUY_ORANGE:
                price = 400;
                break;
            case BUY_BANANA:
                price = 100;
                break;
            default:
                return 0xF0;
            }
            snprintf(text_buffer, sizeof(text_buffer), "You want %s, which costs %d yen. Would you like to buy it?", item_array_purchase_decision_options[dlgflow->result], price);
            return 0x22;
        }
    },
    { .id=0x22, .type=MGC_DLG_TYPE_TEXT, .params= {.text=text_buffer}, .flags=0x01 },
    { .id=0x23, .type=MGC_DLG_TYPE_SELECT, .params= {.items = &items_yesno },
        .cb_before_switch_node = [](const mgc_dlgflow_t *dlgflow) -> mgc_node_id_t { return ( dlgflow->result == 0 ) ? 0x24 : 0x20; }
    },
    { .id=0x24, .type=MGC_DLG_TYPE_TEXT, .params={.text="Thank you. Would you like to buy something else?" }, .flags=0x01 },
    { .id=0x25, .type=MGC_DLG_TYPE_SELECT, .params= {.items = &items_yesno },
        .cb_before_switch_node = [](const mgc_dlgflow_t *dlgflow) -> mgc_node_id_t { return ( dlgflow->result == 0 ) ? 0x20 : 0xF0; }
    },
    { .id=0xF0, .type=MGC_DLG_TYPE_TEXT, .params={.text="Thank you for your purchase. Please come again."}, .terminal=true },
};

static mgc_dialoguebox_t dialoguebox;
static mgc_selectbox_t selectbox;

static void init_handler_components(void);
static void show_dialoguebox(const char *text, uint32_t flags);
static bool proc_dialoguebox(int32_t *result);
static void close_dialoguebox(void);
static void show_selectbox(const char **item_array, size_t item_count, uint32_t flags);
static bool proc_selectbox(int32_t *result);
static void close_selectbox(void);

static const mgc_dlgflow_if_t handler = {
    .show_dialoguebox = show_dialoguebox,
    .proc_dialoguebox = proc_dialoguebox,
    .close_dialoguebox = close_dialoguebox,
    .show_selectbox = show_selectbox,
    .proc_selectbox = proc_selectbox,
    .close_selectbox = close_selectbox
};

int main(void) {
    mgc_pixelbuffer_t pixelbuffer;
    mgc_dlgflow_t dlgflow;
    const mgc_display_if_t *display;
    const mgc_gamepad_if_t *gamepad;
    uint16_t width, height;
    bool loop;

    // Get the display driver.
    display = sys_get_display_driver();

    // Get the screen size.
    width = display->get_width();
    height = display->get_height();

    // Initialize the display driver.
    display->init();

    // Get the gamepad driver.
    gamepad = sys_get_gamepad_driver();

    // Initialized the gamepad driver.
    gamepad->init();

    // Initialize pixel buffer.
    pixelbuffer_init(&pixelbuffer);

    // Sets the entire screen to black.
    pixelbuffer_fill_with_color(&pixelbuffer, MGC_COLOR_BLACK);
    for ( int16_t cell_y = 0; cell_y < height; cell_y += MGC_CELL2PIXEL(1) ) {
        for ( int16_t cell_x = 0; cell_x < width; cell_x += MGC_CELL2PIXEL(1) ) {
            pixelbuffer_draw_cell(&pixelbuffer, display, cell_x, cell_y);
        }
    }

    // Set the background color of the pixel buffer.
    pixelbuffer_set_back_color(&pixelbuffer, MGC_COLOR_BLACK);

    // Initialize the handler for registering with dlgflow.
    init_handler_components();

    // Initialize the dlgflow.
    dlgflow_init(&dlgflow, &handler);

    // Set the node array to dlgflow with the specified start node ID.
    dlgflow_set_node_array(&dlgflow, node_array, countof(node_array), 0);

    loop = true;
    do {

        // Update input information from buttons and joysticks.
        sys_gamepad_proc();

        // Execute processing of the node. Returns true upon completion.
        if ( dlgflow_run_node_proc(&dlgflow) ) {
            // Check if the flow is terminated.
            if ( dlgflow_is_flow_terminated(&dlgflow) ) {
                // The ID of the terminal node is intended to be used as a condition for triggering events and altering processing.
                // (In this example, this node_id is not used.)
                mgc_node_id_t node_id = dlgflow_get_current_node_id(&dlgflow);
                loop = false;
            } else {
                // Switch to the next node.
                dlgflow_switch_to_next_node(&dlgflow);
            }
        } 
        // Draw the entire screen.
        for ( int16_t cell_y = 0; cell_y < height; cell_y += MGC_CELL2PIXEL(1) ) {
            for ( int16_t cell_x = 0; cell_x < width; cell_x += MGC_CELL2PIXEL(1) ) {
                dialoguebox_apply_cell_blending(&dialoguebox, &pixelbuffer, cell_x, cell_y);
                selectbox_apply_cell_blending(&selectbox, &pixelbuffer, cell_x, cell_y);
                pixelbuffer_draw_cell(&pixelbuffer, display, cell_x, cell_y);
            }
        }
    } while (loop == true);

    while (1);

    return 0;
}

static void init_handler_components(void) {

    // Initialize the dialoguebox.
    dialoguebox_init(&dialoguebox, 0, sys_get_font(), false);
    dialoguebox_set_position(&dialoguebox, 8, 8);
    dialoguebox_set_width(&dialoguebox, 224);
    dialoguebox_set_height(&dialoguebox, 12*4);
    dialoguebox_set_cursor_speed(&dialoguebox, 4);
    dialoguebox_set_scroll_speed(&dialoguebox, 6);
    dialoguebox_set_scroll_line(&dialoguebox, 3);
    dialoguebox_set_line_spacing(&dialoguebox, 8);
    dialoguebox_adjust_height(&dialoguebox);
    dialoguebox_set_text(&dialoguebox, "");
    dialoguebox_set_enabled(&dialoguebox, false);

    // Initialize the selectbox.
    selectbox_init(&selectbox, 0, sys_get_font(), false);
    selectbox_set_position(&selectbox, 8, 60);
    selectbox_set_width(&selectbox, 8*6);
    selectbox_set_enabled(&selectbox, false);
}

static void show_dialoguebox(const char *text, uint32_t flags) {
    auto_switch_flag = ((flags&0x01) != 0);
    dialoguebox_set_text(&dialoguebox, text);
    dialoguebox_set_enabled(&dialoguebox, true);
}

static bool proc_dialoguebox(int32_t *result) {
    enum mgc_display_text_state state;
    (void)result;

    dialoguebox_display_update(&dialoguebox);
    state = dialoguebox_get_display_text_state(&dialoguebox);
    if ( state == MGC_DISPLAY_TEXT_STATE_TEXT_END ) {
        return (sys_get_gamepad_driver()->is_key_on_edge(GP_KEY_1)||auto_switch_flag);
    } else {
        return false;
    }
}

static void close_dialoguebox(void) {
    dialoguebox_display_clear(&dialoguebox);
    dialoguebox_set_enabled(&dialoguebox, false);
}

static void show_selectbox(const char **item_array, size_t item_count, uint32_t flags) {
    (void)flags;
    selectbox_clear_items(&selectbox);
    for ( size_t idx = 0; idx < item_count; idx++ ) {
        selectbox_append_item(&selectbox, item_array[idx]);
    }
    selectbox_set_selected_idx(&selectbox, 0);
    selectbox_set_enabled(&selectbox, true);
}

static bool proc_selectbox(int32_t *result) {
    const mgc_gamepad_if_t *gamepad;
    gamepad = sys_get_gamepad_driver();
    if ( gamepad->get_firing_state(GP_KEY_0) ) {
        selectbox_change_selected_idx(&selectbox, true);
    }
    if ( gamepad->is_key_on_edge(GP_KEY_1) ) {
        *result = selectbox.selected_idx;
        return true;
    } else {
        return false;
    }
}

static void close_selectbox(void) {
    selectbox_set_enabled(&selectbox, false);
}

