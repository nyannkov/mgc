#ifndef MGC_MAIN_FRAME_HPP
#define MGC_MAIN_FRAME_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene.hpp"
#include "game_context/game_context.hpp"

namespace app {

template <uint16_t Width, uint16_t Height, typename DisplayDriverT>
struct MainFrame {
    MainFrame(uint16_t x, uint16_t y, DisplayDriverT& display_driver) 
        : scene_(nullptr),
          x_(x),
          y_(y),
          dfb_(buffer_0_, buffer_1_, Width, Height),
          renderer_(dfb_, display_driver, nullptr) { }
    ~MainFrame() = default;
    MainFrame(const MainFrame&) = delete;
    MainFrame& operator = (const MainFrame&) = delete;
    MainFrame(MainFrame&&) = default;
    MainFrame& operator = (MainFrame&&) = default;

    bool set_scene(SceneId id, GameContext& ctx) {
        if ( scene_ ) {
            destroy_current_scene();
        }
        scene_ = create_scene(id, ctx);
        if ( scene_ ) {
            scene_->init();
            return true;
        } else {
            return false;
        }
    }

    void change_next_scene(GameContext& ctx) {
        if ( scene_ && scene_->has_scene_change_request() ) {
            set_scene(scene_->id_next(), ctx);
        }
    }

    bool update_scene(GameContext& ctx) {
        if ( scene_ ) {
            scene_->update();
            return scene_->has_scene_change_request();
        } else {
            return false;
        }
    }

    SceneId scene_id_next() const {
        if ( scene_ ) {
            return scene_->id_next();
        } else {
            return SceneId::Id_000;
        }
    }

    //TODO fade out/in
    void draw_to_buffer() {
        if ( scene_ ) {
            scene_->draw(dfb_.back());
        }
    }

    void flush() {
        renderer_.transfer_to_display_async_at(x_, y_);
    }

    void sync() {
        renderer_.wait_until_idle_interrupt();
    }

private:
    SceneBase *scene_;
    const uint16_t x_;
    const uint16_t y_;
    mgc::graphics::Color buffer_0_[Width*Height];
    mgc::graphics::Color buffer_1_[Width*Height];
    mgc::graphics::DoubleFramebuffer dfb_;
    mgc::render::DoubleBufferedRenderer<DisplayDriverT> renderer_;
};

} // namespace app

#endif/*MGC_MAIN_FRAME_HPP*/

