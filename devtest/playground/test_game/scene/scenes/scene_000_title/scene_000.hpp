#ifndef MGC_SCENE_000_HPP
#define MGC_SCENE_000_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_base.hpp"
#include "game_context/game_context.hpp"

namespace app {

struct Scene000 : SceneBase {

    explicit Scene000(GameContext& ctx);
    SceneId id() const override { return id_; }
    SceneId id_next() const override { return id_next_; }
    bool has_scene_change_request() const override { return change_request_; };
    void init() override;
    void update() override;
    void draw(mgc::graphics::Framebuffer& fb) override;

private:
    const GamepadT& gamepad_;
    SoundControllerT& sound_controller_;
    StatusDisplayRequest& status_display_request_;
    const SceneId id_; 
    SceneId id_next_;
    bool change_request_;
    mgc::parts::BasicLabel label_title_;
    mgc::parts::BasicLabel label_title_en_;
    mgc::parts::BasicSelectbox selectbox_menu_;
    bool change_wait_;
};

} // namespace app

#endif/*MGC_SCENE_000_HPP*/

