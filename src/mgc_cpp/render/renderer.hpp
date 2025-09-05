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
#include "mgc_cpp/camera/icamera_follower.hpp"
#include "mgc_cpp/features/has_position.hpp"
#include "mgc_cpp/features/drawable.hpp"

namespace mgc {
namespace render {

template <typename DisplayDriverT>
struct Renderer {

    static_assert(std::is_base_of<mgc::platform::display::DisplayDriver<DisplayDriverT>, DisplayDriverT>::value,
              "DisplayDriverT must inherit from DisplayDriver<DisplayDriverT>");

    Renderer(mgc::graphics::Framebuffer& fb, DisplayDriverT& driver, const mgc::camera::ICameraFollower* follower)
        : fb_(fb), driver_(driver), follower_(follower) { }

    ~Renderer() = default;

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&&) = default;
    Renderer& operator=(Renderer&&) = default;

    void set_follower(const mgc::camera::ICameraFollower* follower) {
        follower_ = follower;
    }

    void set_back_color(mgc::graphics::Color back_color) {
        fb_.set_back_color(back_color);
    }

    bool is_busy() const {
        return driver_.is_busy();
    }

    void wait_until_idle_interrupt() const {
        driver_.wait_until_idle_interrupt();
    }

    void wait_until_idle_polling() const {
        driver_.wait_until_idle_polling();
    }

    bool draw_to_framebuffer(const mgc::features::Drawable* const* drawables, size_t drawables_count, bool clear_before_draw = true) {

        if ( !drawables ) {
            return false;
        }

        auto cam_pos = this->camera_position();

        if ( clear_before_draw ) {
            fb_.clear();
        }

        for ( size_t index = 0; index < drawables_count; index++ ) {
            if ( drawables[index] ) {
                drawables[index]->draw(fb_, cam_pos, nullptr);
            }
        }

        return true;
    }

    bool transfer_to_display_region_blocking(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {

        return driver_.transfer_region_blocking(fb_.data_bytes(), fb_.size(), x0, y0, x1, y1);
    }

    bool transfer_to_display_blocking() {

        return driver_.transfer_full_region_blocking(fb_.data_bytes(), fb_.size());
    }

    bool transfer_to_display_blocking_at(uint16_t offset_x, uint16_t offset_y) {

        return driver_.transfer_region_blocking(
            fb_.data_bytes(),
            fb_.size(),
            offset_x,
            offset_y,
            offset_x + fb_.width() - 1,
            offset_y + fb_.height() - 1 
        );
    }

    /**
    * Renders and starts asynchronous transfer of the framebuffer.
    *
    * Using a single shared framebuffer (for both draw and transfer) is unsafe
    * if a previous transfer is still in progress.
    *
    * This is experimental and may change.
    */
    bool transfer_to_display_async_at(uint16_t offset_x, uint16_t offset_y) {

        if ( is_busy() ) {
            return false;
        }

        return driver_.transfer_region_async_aligned(
            fb_.data_bytes(),
            fb_.size(),
            offset_x,
            offset_y,
            offset_x + fb_.width() - 1,
            offset_y + fb_.height() - 1
        );
    }

    bool transfer_to_display_async() {

        if ( is_busy() ) {
            return false;
        }
        
        return driver_.transfer_full_region_async(fb_.data_bytes(), fb_.size());
    }

    const mgc::graphics::Framebuffer& framebuffer() const {
        return fb_;
    }

    mgc::graphics::Framebuffer& framebuffer() {
        return fb_;
    }

private:
    mgc::graphics::Framebuffer& fb_;
    DisplayDriverT& driver_;
    const mgc::camera::ICameraFollower* follower_;

    mgc::math::Vec2i camera_position() {
        if ( follower_ ) {
            return follower_->follow_position();
        } else {
            return mgc::math::Vec2i(0, 0);
        }
    }
};

}// namespace render
}// namespace mgc

#endif/*MGC_RENDER_RENDERER_HPP*/
