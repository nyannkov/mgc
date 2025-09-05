/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_RENDER_DOUBLE_BUFFERED_RENDERER_HPP
#define MGC_RENDER_DOUBLE_BUFFERED_RENDERER_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/platform/display/display_driver.hpp"
#include "mgc_cpp/camera/icamera_follower.hpp"
#include "mgc_cpp/features/has_position.hpp"
#include "mgc_cpp/features/drawable.hpp"
#include "mgc_cpp/graphics/double_framebuffer.hpp"

namespace mgc {
namespace render {

template <typename DisplayDriverT>
struct DoubleBufferedRenderer {
    enum class SwapMode {
        Auto,
        Manual
    };

    static_assert(std::is_base_of<mgc::platform::display::DisplayDriver<DisplayDriverT>, DisplayDriverT>::value,
              "DisplayDriverT must inherit from DisplayDriver<DisplayDriverT>");

    DoubleBufferedRenderer(mgc::graphics::DoubleFramebuffer& dfb, DisplayDriverT& driver, const mgc::camera::ICameraFollower* follower)
        : dfb_(dfb), driver_(driver), follower_(follower) { }

    ~DoubleBufferedRenderer() = default;

    DoubleBufferedRenderer(const DoubleBufferedRenderer&) = delete;
    DoubleBufferedRenderer& operator=(const DoubleBufferedRenderer&) = delete;
    DoubleBufferedRenderer(DoubleBufferedRenderer&&) = default;
    DoubleBufferedRenderer& operator=(DoubleBufferedRenderer&&) = default;

    void set_follower(const mgc::camera::ICameraFollower* follower) {
        follower_ = follower;
    }

    void set_back_color(mgc::graphics::Color back_color) {
        dfb_.set_back_color(back_color);
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
            dfb_.back().clear();
        }

        for ( size_t index = 0; index < drawables_count; index++ ) {
            if ( drawables[index] ) {
                drawables[index]->draw(dfb_.back(), cam_pos, nullptr);
            }
        }

        return true;
    }

    bool transfer_to_display_async_at(uint16_t offset_x, uint16_t offset_y, SwapMode mode = SwapMode::Auto) {

        if ( is_busy() ) {
            return false;
        }

        const mgc::graphics::Framebuffer& fb = dfb_.back();

        bool success = driver_.transfer_region_async_aligned(
            fb.data_bytes(),
            fb.size(),
            offset_x,
            offset_y,
            offset_x + fb.width() - 1,
            offset_y + fb.height() - 1
        );

        if ( success && mode == SwapMode::Auto) {
            dfb_.swap();
        }
        
        return success;
    }

    bool transfer_to_display_async(SwapMode mode = SwapMode::Auto) {

        if ( is_busy() ) {
            return false;
        }
        
        const mgc::graphics::Framebuffer& fb = dfb_.back();

        bool success = driver_.transfer_full_region_async(fb.data_bytes(), fb.size());

        if ( success && mode == SwapMode::Auto) {
            dfb_.swap();
        }
        
        return success;
    }

    bool transfer_to_display_blocking_at(uint16_t offset_x, uint16_t offset_y, SwapMode mode = SwapMode::Manual) {

        const mgc::graphics::Framebuffer& fb = dfb_.back();

        bool success = driver_.transfer_region_blocking(
            fb.data_bytes(),
            fb.size(),
            offset_x,
            offset_y,
            offset_x + fb.width() - 1,
            offset_y + fb.height() - 1
        );

        if ( success && mode == SwapMode::Auto) {
            dfb_.swap();
        }
        
        return success;
    }

    bool transfer_to_display_blocking(SwapMode mode = SwapMode::Manual) {
        
        const mgc::graphics::Framebuffer& fb = dfb_.back();

        bool success = driver_.transfer_full_region_blocking(fb.data_bytes(), fb.size());

        if ( success && mode == SwapMode::Auto) {
            dfb_.swap();
        }
        
        return success;
    }

    bool swap() {
        if ( is_busy() ) {
            return false;
        }
        dfb_.swap();
        return true;
    }

    const mgc::graphics::DoubleFramebuffer& double_buffer() const {
        return dfb_;
    }

    mgc::graphics::DoubleFramebuffer& double_buffer() {
        return dfb_;
    }

private:
    mgc::graphics::DoubleFramebuffer& dfb_;
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

#endif/*MGC_RENDER_DOUBLE_BUFFERED_RENDERER_HPP*/
