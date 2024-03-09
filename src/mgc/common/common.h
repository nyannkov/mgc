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

#ifndef NULL
#define NULL    ((void *)0)
#endif/*NULL*/

#ifndef MGC_INFO
#define MGC_INFO(str)
#endif/*MGC_INFO*/

#ifndef MGC_WARN
#define MGC_WARN(str)
#endif/*MGC_WARN*/

#ifndef MGC_DEFAULT_ENABLED
#define MGC_DEFAULT_ENABLED     true
#endif/*MGC_DEFAULT_ENABLED*/

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

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_COMMON_H*/
