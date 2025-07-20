/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_FIXED_DOUBLE_BUFFERED_SCREEN_HPP
#define MGC_FIXED_DOUBLE_BUFFERED_SCREEN_HPP

#include "mgc_cpp/mgc.hpp"

namespace app {

template <
    uint16_t X,
    uint16_t Y,
    uint16_t Width,
    uint16_t Height,
    typename DisplayDriverT,
    size_t MaxDrawableCount = 16,
    size_t MaxUpdatableCount = 16
>
struct FixedDoubleBufferedScreen {
    using ColorT = mgc::graphics::Color;
    using DoubleFramebufferT = mgc::graphics::DoubleFramebuffer;
    using FramebufferT = mgc::graphics::Framebuffer;
    using RendererT = mgc::render::DoubleBufferedRenderer<DisplayDriverT>;
    using CameraT = mgc::camera::SimpleCameraFollower;
    using DrawableT = mgc::features::Drawable;

    explicit FixedDoubleBufferedScreen(DisplayDriverT& display_driver)
        : dfb_(buffer_0_, buffer_1_, Width, Height),
          renderer_(dfb_, display_driver, &camera_),
          drawable_count_(0)
          { }

    ~FixedDoubleBufferedScreen() = default;
    FixedDoubleBufferedScreen(const FixedDoubleBufferedScreen&) = delete;
    FixedDoubleBufferedScreen& operator=(const FixedDoubleBufferedScreen&) = delete;
    FixedDoubleBufferedScreen(FixedDoubleBufferedScreen&&) = default;
    FixedDoubleBufferedScreen& operator=(FixedDoubleBufferedScreen&&) = default;

    bool register_drawable(const DrawableT* drawable) {

        if ( !drawable ) { return false; }

        if ( drawable_count_ < drawables_.size() ) {
            drawables_[drawable_count_++] = drawable;
            return true;
        }
        return false;
    }

    void unregister_all_drawables() {
        for ( size_t i = 0; i < drawable_count_; i++ ) {
            drawables_[i] = nullptr;
        }
    }

    void draw_to_buffer() {

        renderer_.draw_to_framebuffer(drawables_.data(), drawable_count_);
    }

    void sync() {

        renderer_.wait_until_idle_interrupt();
    }

    void flush() {

        renderer_.transfer_to_display_async_at(X, Y);
    }

    FramebufferT& back() {
        return dfb_.back();
    }

    const FramebufferT& front() const {
        return dfb_.front();
    }

    CameraT& camera() {
        return camera_;
    }

private:

    std::array<const DrawableT*, MaxDrawableCount> drawables_;
    size_t drawable_count_;
    ColorT buffer_0_[Width*Height];
    ColorT buffer_1_[Width*Height];
    DoubleFramebufferT dfb_;
    RendererT renderer_;
    CameraT camera_;
};

} // namepsace app

#endif/*MGC_FIXED_DOUBLE_BUFFERED_SCREEN_HPP*/

