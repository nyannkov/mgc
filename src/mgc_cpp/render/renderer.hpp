/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_RENDER_RENDERER_HPP
#define MGC_RENDER_RENDERER_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/platform/display/display_driver.hpp"
#include "mgc_cpp/graphics/framebuffer.hpp"
#include "mgc_cpp/camera/icamera_follower.hpp"
#include "mgc_cpp/features/positionable.hpp"
#include "mgc_cpp/features/drawable.hpp"
#include "mgc_cpp/features/cell_drawable.hpp"

namespace mgc {
namespace render {

template <typename DisplayDriverT>
struct Renderer {

    static_assert(std::is_base_of<mgc::platform::display::DisplayDriver<DisplayDriverT>, DisplayDriverT>::value,
              "DisplayDriverT must inherit from DisplayDriver<DisplayDriverT>");

    Renderer(DisplayDriverT& driver, const mgc::camera::ICameraFollower* follower)
        : driver_(driver), follower_(follower) { }

    ~Renderer() = default;

    void set_follower(const mgc::camera::ICameraFollower* follower) {
        follower_ = follower;
    }

    void set_back_color(mgc::graphics::Color back_color) {
        back_color_ = back_color;
    }

    void render(mgc::graphics::Framebuffer& fb, const mgc::features::Drawable** drawables, size_t drawable_count, bool buffer_clear = true) {
        if ( !drawables ) {
            return;
        }

        mgc::geometry::Point cam_pos = this->camera_position();
        for ( size_t index = 0; index < drawable_count; index++ ) {
            if ( drawables[index] ) {
                drawables[index]->draw(fb, cam_pos, nullptr);
            }
        }
        driver_.transfer_region(
                    fb.data_bytes(),
                    fb.size(),
                    0,
                    0,
                    fb.width() - 1,
                    fb.height() -1
        );

        if ( buffer_clear ) {
            fb.set_back_color(back_color_);
            fb.clear();
        }
    }

    void render(mgc::graphics::Framebuffer& fb, bool buffer_clear = true) {
        driver_.transfer_region(
                    fb.data_bytes(),
                    fb.size(),
                    0,
                    0,
                    fb.width() - 1,
                    fb.height() -1
        );
        if ( buffer_clear ) {
            fb.set_back_color(back_color_);
            fb.clear();
        }
    }

    void render(mgc::graphics::CellBuffer& cb, const mgc::features::CellDrawable** drawables, size_t drawable_count) {
        if ( !drawables ) {
            return;
        }

        uint16_t width = driver_.width();
        uint16_t height = driver_.height();
        mgc::geometry::Point cam_pos = this->camera_position();
        cb.set_back_color(back_color_);

        for ( uint16_t y = 0; y < height; y += MGC_CELL2PIXEL(1)) {
            for ( uint16_t x = 0; x < width; x += MGC_CELL2PIXEL(1)) {
                for ( size_t index = 0; index < drawable_count; index++ ) {
                    if ( drawables[index] ) {
                        drawables[index]->cell_draw(cb, x, y, cam_pos, nullptr);
                    }
                }
                driver_.transfer_region(
                            cb.data_bytes(),
                            cb.size(),
                            x,
                            y,
                            x + MGC_CELL_LEN - 1,
                            y + MGC_CELL_LEN - 1
                );
                cb.clear();
            }
        }
    }

private:
    DisplayDriverT& driver_;
    const mgc::camera::ICameraFollower* follower_;
    mgc::graphics::Color back_color_;

    mgc::geometry::Point camera_position() {
        if ( follower_ ) {
            return follower_->get_follow_position();
        } else {
            return mgc::geometry::Point(0, 0);
        }
    }
};

}// namespace render
}// namespace mgc

#endif/*MGC_RENDER_RENDERER_HPP*/
