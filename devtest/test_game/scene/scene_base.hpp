#ifndef MGC_SCENE_BASE_HPP
#define MGC_SCENE_BASE_HPP

#include "mgc_cpp/mgc.hpp"
#include "game_context/game_context.hpp"
#include "scene_context.hpp"
#include "interface/iscene.hpp"
#include "utils/game_over_effect/game_over_effect.hpp"

namespace app {

struct SceneBase : IScene {
    explicit SceneBase(GameContext& ctx)
        : id_prev_(ctx.scene_info().prev_scene_id()),
          scx_ {
            ctx.sound_controller(),
            ctx.gamepad(),
            ctx.frame_timer(),
            ctx.player(),
            ctx.default_talkflow_effects(),
            ctx.checkpoint_info(),
            nullptr,
            nullptr,
            nullptr
          },
          talkflow_(ctx.gamepad()),
          status_display_request_(ctx.status_display_request()),
          game_over_effect_(ctx.player(), ctx.frame_timer(), ctx.sound_controller()) {

        load_default_config(talkflow_);
        talkflow_.bind_effects(scx_.talkflow_effects);
    }
    virtual ~SceneBase() = default;

    SceneId id() const override {
        return id_;
    }

    SceneId id_next() const override {
        return id_next_;
    }

    bool has_scene_change_request() const override {
        return scene_change_request_;
    }

    void set_menu_request() override {
        menu_request_ = true;
    }

    void clear_menu_request() override {
        menu_request_ = false;
    }

    bool has_menu_request() const override {
        return menu_request_;
    }

protected:
    SceneId id_prev() const { return id_prev_; }
    SceneContext scx_;
    TalkflowControllerT talkflow_;
    StatusDisplayRequest& status_display_request_;
    GameOverEffect game_over_effect_;

    void set_id(SceneId id) { id_ = id; }
    void set_scene_change_request(SceneId id_next) { 
        id_next_ = id_next;
        scene_change_request_ = true;
    }

private:
    const SceneId id_prev_;
    bool menu_request_ = false;
    bool scene_change_request_ = false;
    SceneId id_ = SceneId::Title;
    SceneId id_next_ = SceneId::Title;
};

} // namespace app

#endif/*MGC_SCENE_BASE_HPP*/

