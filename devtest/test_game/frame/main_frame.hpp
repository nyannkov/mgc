#ifndef MGC_MAIN_FRAME_HPP
#define MGC_MAIN_FRAME_HPP

#include "game_context.hpp"
#include "scene/scene_manager.hpp"

namespace app {

template <uint16_t Width, uint16_t Height>
struct MainFrame {
    MainFrame(uint16_t x, uint16_t y, GameContext& ctx) 
        : x_(x),
          y_(y),
          dfb_(buffer_0_, buffer_1_, Width, Height),
          renderer_(dfb_, ctx.platform.display_driver, nullptr),
          scene_manager_(ctx) { }
    ~MainFrame() = default;
    MainFrame(const MainFrame&) = delete;
    MainFrame& operator = (const MainFrame&) = delete;
    MainFrame(MainFrame&&) = default;
    MainFrame& operator = (MainFrame&&) = default;

    void init() {
        scene_manager_.init();
    }

    void update_screen() {
        scene_manager_.update();
    }

    void draw_to_buffer() {
        scene_manager_.draw(dfb_.back());
    }

    void flush() {
        renderer_.transfer_to_display_async_at(x_, y_);
    }

    void sync() {
        renderer_.wait_until_idle_interrupt();
    }

private:
    const uint16_t x_;
    const uint16_t y_;
    mgc::graphics::Color buffer_0_[Width*Height];
    mgc::graphics::Color buffer_1_[Width*Height];
    mgc::graphics::DoubleFramebuffer dfb_;
    mgc::render::DoubleBufferedRenderer<DisplayDriverT> renderer_;
    SceneManager scene_manager_;
};

} // namespace app

#endif/*MGC_MAIN_FRAME_HPP*/

