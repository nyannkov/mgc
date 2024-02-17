/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "textblock.h"

static inline enum mgc_display_text_state display_update(mgc_textblock_t *textblock) {
    const char *next_cursor;
    int16_t scale;
    switch (textblock->state) {
    case MGC_DISPLAY_TEXT_STATE_INIT:
        textblock->cursor = textblock->text;
        textblock->state = MGC_DISPLAY_TEXT_STATE_CURSOR_MOVING;
        break;
    case MGC_DISPLAY_TEXT_STATE_CURSOR_MOVING:
        if ( encoding_utf8_get_next_char(textblock->cursor, &next_cursor) == true ) {
            textblock->cursor = next_cursor;
            if ( textblock->current_line < textblock->line_count ) {
                if ( next_cursor == textblock->lines[textblock->current_line+1] ) {
                    textblock->current_line++;
                    if ( textblock->current_line >= textblock->line_count ) {
                        textblock->state = MGC_DISPLAY_TEXT_STATE_TEXT_END;
                    } else if ( textblock->current_line >= textblock->scroll_line ) {
                        if ( (textblock->line_ofs + textblock->scroll_line) < textblock->line_count ) {
                            textblock->state = MGC_DISPLAY_TEXT_STATE_TEXT_SCROLLING;
                        }
                    } else {
                    }
                }
            } else {
                textblock->state = MGC_DISPLAY_TEXT_STATE_TEXT_END;
            }
        } else {
            textblock->state = MGC_DISPLAY_TEXT_STATE_TEXT_END;
        }
        break;
    case MGC_DISPLAY_TEXT_STATE_TEXT_SCROLLING:
        textblock->scroll_y += textblock->scroll_speed;
        scale = textblock->fontsize2x ? 2 : 1;
        if ( textblock->scroll_y >= (textblock->font->fbb_y*scale+(uint16_t)textblock->line_spacing) ) {
            textblock->scroll_y = 0;
            textblock->line_ofs++;
            textblock->state = MGC_DISPLAY_TEXT_STATE_CURSOR_MOVING;
        }
        break;
    case MGC_DISPLAY_TEXT_STATE_TEXT_END:
        break;
    default:
        textblock->state = MGC_DISPLAY_TEXT_STATE_INIT;
        break;
    }
    return textblock->state;
}

void textblock_init(mgc_textblock_t *textblock, mgc_id_t id, const mgc_font_t *font, bool fontsize2x) {
    if ( ( textblock == NULL ) ||
         ( font == NULL ) ||
         ( font->fbb_x > MGC_FONT_MAX_FONT_SIZE ) ||
         ( font->fbb_y > MGC_FONT_MAX_FONT_SIZE )
    ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock->id = id;
    textblock->x = 0;
    textblock->y = 0;
    textblock->r_cell_x_ofs = 1;
    textblock->r_cell_y_ofs = 1;
    textblock->width = MGC_CELL_LEN;
    textblock->height = MGC_CELL_LEN;
    textblock->text = NULL;
    textblock->font = font;
    textblock->fore_color = MGC_COLOR_SWAP(MGC_COLOR_WHITE);
    textblock->back_color = MGC_COLOR_SWAP(MGC_COLOR_BLACK);
    textblock->enable_back_color = false;
    textblock->fontsize2x = fontsize2x;
    textblock->enabled = MGC_DEFAULT_ENABLED;
    for ( size_t i = 0; i < MGC_TEXTBLOCK_MAX_LINES; i++ ) {
        textblock->lines[i] = NULL;
    }
    textblock->line_count = 0;
    textblock->cursor = NULL;
    textblock->line_ofs = 0;
    textblock->current_line = 0;
    textblock->cursor_speed = 1;
    textblock->scroll_speed = 1;
    textblock->scroll_y = 0;
    textblock->scroll_line = 3;
    textblock->line_spacing = 0;
    textblock->state = MGC_DISPLAY_TEXT_STATE_INIT;
}

void textblock_set_position(mgc_textblock_t *textblock, int16_t x, int16_t y) {
    if ( textblock == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock->x = x;
    textblock->y = y;
}

void textblock_set_enabled(mgc_textblock_t *textblock, bool enabled) {
    if ( textblock == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock->enabled = enabled;
}

void textblock_set_text(mgc_textblock_t *textblock, const char *text) {
    const char * p;
    const char * tmp_p;
    uint32_t unicode;
    int16_t total_dwx0;
    int16_t scale;

    if ( textblock == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }

    scale = textblock->fontsize2x ? 2 : 1;

    textblock->text = text;
    textblock->cursor = text;
    textblock->line_ofs = 0;
    textblock->line_count = 0;
    textblock->current_line = 0;
    textblock->scroll_y = 0;
    textblock->state = MGC_DISPLAY_TEXT_STATE_INIT;
    p = text;
    for ( size_t index = 0; index < MGC_TEXTBLOCK_MAX_LINES; index++ ) {
        textblock->lines[index] = p;
        if (*p) {
            textblock->line_count++;
        }
        total_dwx0 = 0;
        while (*p) {
            unicode = encoding_utf8_to_unicode(p, &p);
            if ( unicode == 0x0D ) {
                unicode = encoding_utf8_to_unicode(p, &tmp_p);
                if ( unicode == 0x0A ) {
                    p = tmp_p;
                }
                break;
            } else if ( (unicode == 0x00) || (unicode == 0x0A) ) {
                break;
            } else {
                const mgc_glyph_t *glyph;
                glyph = font_get_glyph_info(textblock->font, unicode);
                total_dwx0 += (glyph->dwx0*scale);
                if ( textblock->width <= total_dwx0 ) {
                    break;
                }
            }
        }
    }
}

void textblock_set_height(mgc_textblock_t *textblock, uint16_t height) {
    if ( textblock == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock->height = height;
}

void textblock_set_width(mgc_textblock_t *textblock, uint16_t width) {
    if ( textblock == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock->width = width;
    /* Adjustment of text line breaks. */
    textblock_set_text(textblock, textblock->text);
}

void textblock_set_fore_color(mgc_textblock_t *textblock, mgc_color_t fore_color) {
    if ( textblock == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock->fore_color = MGC_COLOR_SWAP(fore_color);
}

void textblock_set_back_color(mgc_textblock_t *textblock, mgc_color_t back_color) {
    if ( textblock == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock->back_color = MGC_COLOR_SWAP(back_color);
}

void textblock_set_enable_back_color(mgc_textblock_t *textblock, bool enable) {
    if ( textblock == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock->enable_back_color = enable;
}

void textblock_set_cursor_speed(mgc_textblock_t *textblock, uint8_t cursor_speed) {
    if ( textblock == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock->cursor_speed = cursor_speed;
}

void textblock_set_scroll_speed(mgc_textblock_t *textblock, uint8_t scroll_speed) {
    if ( textblock == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock->scroll_speed = scroll_speed;
}

void textblock_set_scroll_line(mgc_textblock_t *textblock, uint8_t scroll_line) {
    if ( textblock == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock->scroll_line = scroll_line;
}

void textblock_set_line_spacing(mgc_textblock_t *textblock, uint8_t line_spacing) {
    if ( textblock == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock->line_spacing = line_spacing;
}

void textblock_set_r_cell_offset(mgc_textblock_t *textblock, uint8_t r_cell_x_ofs, uint8_t r_cell_y_ofs) {
    if ( textblock == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock->r_cell_x_ofs = r_cell_x_ofs;
    textblock->r_cell_y_ofs = r_cell_y_ofs;
}

void textblock_display_update(mgc_textblock_t *textblock) {
    uint8_t cursor_move_count = textblock->cursor_speed;
    enum mgc_display_text_state state;
    if ( textblock == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    for ( size_t n = 0; n < textblock->cursor_speed; n++ ) {
        state = display_update(textblock);
        if ( state != MGC_DISPLAY_TEXT_STATE_CURSOR_MOVING ) {
            break;
        }
    }
}

void textblock_display_clear(mgc_textblock_t *textblock) {
    if ( textblock == NULL ) {
        MGC_WARN("Invalid handler");
        return;
    }
    textblock_set_text(textblock, "");
}

enum mgc_display_text_state textblock_get_display_text_state(const mgc_textblock_t *textblock) {
    if ( textblock == NULL ) {
        MGC_WARN("Invalid handler");
        return MGC_DISPLAY_TEXT_STATE_INIT;
    }
    return textblock->state;
}

bool textblock_apply_cell_blending(const mgc_textblock_t *textblock, mgc_pixelbuffer_t *pixelbuffer, int16_t cell_x, int16_t cell_y) {
    int16_t l0, l1;
    int16_t r0, r1;
    int16_t t0, t1;
    int16_t b0, b1;
    const char *p;
    const char *tmp_p;
    int16_t dx, dy;
    int16_t scale;
    int16_t shift;

    if ( ( textblock == NULL )         ||
         ( textblock->font == NULL )   ||
         ( textblock->cursor == NULL ) ||
         ( pixelbuffer == NULL )
    ) {
        MGC_WARN("Invalid handler");
        return false;
    }
    if ( textblock->enabled == false ) {
        MGC_INFO("Handler is disabled");
        return false;
    }
    if ( textblock->state == MGC_DISPLAY_TEXT_STATE_INIT ) {
        MGC_INFO("Bfore text display begins");
        return false;
    }

    l1 = cell_x;
    t1 = cell_y;
    if ( textblock->r_cell_x_ofs != 0 ) {
        l1 += pixelbuffer->cell_x_ofs / textblock->r_cell_x_ofs;
    }
    if ( textblock->r_cell_y_ofs != 0 ) {
        t1 += pixelbuffer->cell_y_ofs / textblock->r_cell_y_ofs;
    }
    r1 = l1 + MGC_CELL_LEN - 1;
    b1 = t1 + MGC_CELL_LEN - 1;


    if ( (r1 <= textblock->x) ||
         (b1 <= textblock->y) ||
         ((textblock->x+textblock->width) <= l1) ||
         ((textblock->y+textblock->height) <= t1)
    ) {
        /* Rendering not required */
        return false;
    }

    scale = textblock->fontsize2x ? 2 : 1;
    shift = scale-1;

    dy = textblock->font->fbb_y*scale;
    t0 = textblock->y - textblock->scroll_y;
    b0 = t0+dy-1;
    for ( uint16_t index = textblock->line_ofs; index < MGC_TEXTBLOCK_MAX_LINES; index++ ) {
        if ( b0 >= (t0 + textblock->height) ) {
            break;
        }
        if ( (t1>=b0) || (t0>=b1) ) {
            t0 += (dy + textblock->line_spacing);
            b0 += (dy + textblock->line_spacing);
            continue;
        }

        l0 = textblock->x;
        r0 = l0;
        p = textblock->lines[index];
        while ((*p)&&(p <= textblock->cursor)) {
            uint32_t unicode;
            unicode = encoding_utf8_to_unicode(p, &p);
            if ( unicode == 0x0D ) {
                unicode = encoding_utf8_to_unicode(p, &tmp_p);
                if ( unicode == 0x0A ) {
                    p = tmp_p;
                }
                break;
            } else if ( (unicode == 0x00) || (unicode == 0x0A) ) {
                break;
            } else {
                const mgc_glyph_t *glyph;
                glyph = font_get_glyph_info(textblock->font, unicode);
                dx = glyph->dwx0*scale;
                r0 = l0 + dx - 1;
                if ( r0 >= (textblock->x + textblock->width) ) {
                    break;
                }
                if ( (l1<=r0) && (l0<=r1) ) {
                    int16_t x, y, wy;
                    int16_t x_s, y_s, x_e, y_e;
                    uint32_t bitmap[MGC_FONT_MAX_FONT_SIZE] = {0};
                    font_load_bitmap(textblock->font, glyph, bitmap, MGC_FONT_MAX_FONT_SIZE);
                    x_s = (( l1 < l0 ) ? l0 : l1) - l0;
                    x_e = (( r1 < r0 ) ? r1 : r0) - l0;
                    y_s = (( t1 < t0 ) ? t0 : t1) - t0;
                    y_e = (( b1 < b0 ) ? b1 : b0) - t0;
                    for ( int16_t X = x_s; X <= x_e; X++ ) {
                        size_t idx, idx_ofs;
                        uint32_t mask_x;
                        if (glyph->bb_w <= 8 ) {
                            mask_x = 0x00010000>>(X>>shift);
                        } else {
                            mask_x = 0x01000000>>(X>>shift);
                        }
                        idx_ofs = MGC_MOD_CELL_LEN(X+l0-l1) << MGC_CELL_LEN_LOG2;
                        for ( int16_t Y = y_s; Y <= y_e; Y++ ) {
                            if ( (bitmap[Y>>shift] & mask_x ) != 0 ) {
                                idx =  idx_ofs + MGC_MOD_CELL_LEN(Y+t0-t1);
                                pixelbuffer->pixelbuf[idx] = textblock->fore_color;
                            } else {
                                if ( textblock->enable_back_color ) {
                                    idx =  idx_ofs + MGC_MOD_CELL_LEN(Y+t0-t1);
                                    pixelbuffer->pixelbuf[idx] = textblock->back_color;
                                }
                            }
                        }
                    }
                }
                l0 = r0 + 1;
            }
        }
        t0 += dy + textblock->line_spacing;
        b0 += dy + textblock->line_spacing;
    }
    return true;
}

