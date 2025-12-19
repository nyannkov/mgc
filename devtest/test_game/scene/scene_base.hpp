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

    virtual SceneId id() const override {
        return id_;
    }
    virtual SceneId id_next() const override {
        return id_next_;
    }

    virtual void set_id_next(SceneId id) override {
        id_next_ = id;
    }

    virtual bool has_scene_change_request() const {
        return scene_change_request_;
    }

protected:
    SceneId id_ = SceneId::Id_000;
    SceneId id_next_ = SceneId::Id_000;
    bool scene_change_request_ = false;
    const SceneId id_prev_;
    SceneContext scx_;
    TalkflowControllerT talkflow_;
    StatusDisplayRequest& status_display_request_;
    GameOverEffect game_over_effect_;
};

} // namespace app

#endif/*MGC_SCENE_BASE_HPP*/

