#include "app_common.hpp"

namespace app {

//TODO library
void fill_color_with_alpha(
    FramebufferT& fb,
    ColorT fill_color,
    uint8_t alpha
) {
    uint16_t width = fb.width();
    uint16_t height = fb.height();
    const ColorT* draw_buf = fb.data();
    ColorT dst = fill_color;

    for ( uint16_t x = 0; x < width; x++ ) {
        for ( uint16_t y = 0; y < height; y++ ) {
            ColorT src = MGC_COLOR_SWAP(draw_buf[x + width*y]);
            uint8_t sr = (src >> 11) & 0x1F;
            uint8_t sg = (src >>  5) & 0x3F;
            uint8_t sb =  src        & 0x1F;
            uint8_t dr = (dst >> 11) & 0x1F;
            uint8_t dg = (dst >>  5) & 0x3F;
            uint8_t db =  dst        & 0x1F;
            uint8_t r = (sr * alpha + dr * (255 - alpha) + 128) >> 8;
            uint8_t g = (sg * alpha + dg * (255 - alpha) + 128) >> 8;
            uint8_t b = (sb * alpha + db * (255 - alpha) + 128) >> 8;
            src = ((ColorT)r<<11)|((ColorT)g<<5)|((ColorT)b<<0);
            fb.draw_pixel(x, y, src);
        }
    }
}


} // namespace app

