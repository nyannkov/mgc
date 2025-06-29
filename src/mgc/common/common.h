/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#ifndef MGC_COMMON_H
#define MGC_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MGC_USE_CUSTOM_MGC_CONF_H
#include "mgc_conf.h"
#else
#include "mgc/mgc_conf.h"
#endif

typedef uint16_t mgc_id_t;

#ifndef countof
#define countof(x)      (sizeof((x))/sizeof((x)[0]))
#endif/*countof*/

#ifndef NULL
#define NULL    ((void *)0)
#endif/*NULL*/

#ifndef MGC_WEAK
#define MGC_WEAK	__attribute__((weak))
#endif/*MGC_WEAK*/

#ifndef MGC_INFO
#define MGC_INFO(str)
#endif/*MGC_INFO*/

#ifndef MGC_WARN
#define MGC_WARN(str)
#endif/*MGC_WARN*/

#ifndef MGC_ASSERT
#define MGC_ASSERT(cond, str)
#endif/*MGC_ASSERT*/

#ifndef MGC_DEFAULT_VISIBLE
#define MGC_DEFAULT_VISIBLE     true
#endif/*MGC_DEFAULT_VISIBLE*/

#ifndef MGC_ABS
#define MGC_ABS(x)              (((x) >= 0) ? (x) : ((x)*-1))
#endif/*MGC_ABS*/

#define MGC_CELL_LEN_LOG2       (4)
#define MGC_CELL_LEN            (1<<MGC_CELL_LEN_LOG2)
#define MGC_CELL_LEN_MOD        (MGC_CELL_LEN-1)

#define MGC_MOD_CELL_LEN(x)     ((x)&MGC_CELL_LEN_MOD)
#define MGC_DIV_CELL_LEN(x)     (((0<=(x))||((x)<=(signed)MGC_CELL_LEN*-1)) ? ((x)>>MGC_CELL_LEN_LOG2) : 0)

#define MGC_CELL2PIXEL(n)       ((n)*MGC_CELL_LEN)

enum mgc_pixel_format {
    MGC_RGB565 = 0,
};

typedef uint16_t mgc_color_rgb565_t;
typedef mgc_color_rgb565_t mgc_color_t;

#define MGC_COLOR(r,g,b)    (((mgc_color_t)(0x1F*(r)+0.5F)<<11) | \
                             ((mgc_color_t)(0x3F*(g)+0.5F)<<5)  | \
                             ((mgc_color_t)(0x1F*(b)+0.5F)<<0))
#define MGC_COLOR_BLACK         0x0000
#define MGC_COLOR_WHITE         0xFFFF

#ifndef MGC_COLOR_SWAP
#ifdef MGC_PIXELBUF_NOSWAP
#define MGC_COLOR_SWAP(c)   (c)
#else
#define MGC_COLOR_SWAP(c)   (((0xFF&(c))<<8)|((c>>8)&0xFF))
#endif
#endif

#ifndef MGC_PIXELBUF_ORDER
#define MGC_PIXELBUF_ORDER     (0U)
#endif

#ifndef MGC_GET_PIXELBUF_INDEX
#if MGC_PIXELBUF_ORDER==0U
#define MGC_GET_PIXELBUF_INDEX(x, y, w, h) \
    (size_t)(h) * (size_t)(x) + (size_t)(y);
#else
#define MGC_GET_PIXELBUF_INDEX(x, y, w, h) \
    (size_t)(w) * (size_t)(y) + (size_t)(x);
#endif
#endif

#ifndef MGC_GET_PIXELBUF_INDEX_OPTIMIZED_16X16
#if MGC_PIXELBUF_ORDER==0U
#define MGC_GET_PIXELBUF_INDEX_OPTIMIZED_16X16(x, y) \
    ((size_t)(x)<<MGC_CELL_LEN_LOG2) + (size_t)(y);
#else
#define MGC_GET_PIXELBUF_INDEX_OPTIMIZED_16X16(x, y) \
    ((size_t)(y)<<MGC_CELL_LEN_LOG2) + (size_t)(x);
#endif
#endif

typedef struct mgc_point {
    int16_t x;
    int16_t y;
} mgc_point_t;

typedef struct mgc_draw_options {
    uint32_t reserved;
} mgc_draw_options_t;

typedef struct mgc_parallax_factor {
    float f_x;
    float f_y;
} mgc_parallax_factor_t;

typedef struct mgc_trim_region {
    uint16_t left;
    uint16_t right;
    uint16_t top;
    uint16_t bottom;
} mgc_trim_region_t;

typedef struct mgc_padding {
    uint8_t left;
    uint8_t right;
    uint8_t top;
    uint8_t bottom;
} mgc_padding_t;

typedef struct mgc_size {
    uint16_t width;
    uint16_t height;
} mgc_size_t;

#define MGC_PARALLAX_SHIFT(coord, factor) (int16_t)((coord) * (factor))

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_COMMON_H*/
