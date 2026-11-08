#ifndef MGC_SCENE_BASE_HPP
#define MGC_SCENE_BASE_HPP

#include "mgc_cpp/mgc.hpp"
#include "game_context.hpp"
#include "scene_context.hpp"
#include "interface/iscene.hpp"
#include "resources/mml/mml.h"
#include "utils/game_over_effect/game_over_effect.hpp"
#include "scene/scene_core.hpp"

namespace app {

struct SceneBase : IScene {
    explicit SceneBase(GameContext& ctx)
        : id_prev_(ctx.world_state.scene_info.prev_scene_id()),
          from_gate_id_(ctx.world_state.scene_info.from_gate_id()),
          talkflow_effects_(ctx.platform.sound_controller),
          scx_ {
            ctx.platform.sound_controller,
            ctx.platform.gamepad,
            ctx.platform.frame_timer,
            ctx.world_state.player,
            talkflow_effects_,
            ctx.world_state,
            nullptr,
            nullptr,
            nullptr
          },
          talkflow_(ctx.platform.gamepad),
          status_display_request_(ctx.request_hub.status_display_request),
          game_over_effect_(
            ctx.world_state.player,
            ctx.platform.frame_timer,
            ctx.platform.sound_controller
          ) {

        load_default_config(talkflow_);
        talkflow_.bind_effects(scx_.talkflow_effects);
    }
    virtual ~SceneBase() = default;

    SceneId id() const override {
        return id_;
    }

    SceneTransitionRequest scene_trans_req_info() const override {
        return scene_trans_req_info_;
    }

    bool has_scene_trans_request() const override {
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
    uint32_t from_gate_id() const { return from_gate_id_; }
    TalkflowControllerT talkflow_;
    TalkflowEffects talkflow_effects_;
    StatusDisplayRequest& status_display_request_;
    GameOverEffect game_over_effect_;
    SceneContext scx_;

    void set_id(SceneId id) { id_ = id; }
    void set_scene_change_request(SceneTransitionRequest request) { 
        scene_trans_req_info_ = request;
        scene_change_request_ = true;
    }

    void set_background_music(int mml_id) {
        
        int current_id = scx_.sound.current_background_music_id();

        if ( current_id != mml_id ) {
            scx_.sound.play_background_music(mml_id);
        }
    }

    void update_common(mgc::camera::ICameraFollower* camera) { //TODO
        SceneTransitionRequest request;
        
        if ( scx_.gamepad.just_pressed(mgc::platform::input::Key::Menu) ) {
            set_menu_request();
            return;
        }

        scene::update(scx_, talkflow_, camera);

        bool r = scene::check_scene_transition_request(
                scx_,
                game_over_effect_,
                request
        );

        if ( r ) {
            set_scene_change_request(request);
        }
    }

private:
    const SceneId id_prev_;
    const uint32_t from_gate_id_;
    int bgm_id_ = MML_BGM_COUNT;
    bool menu_request_ = false;
    bool scene_change_request_ = false;
    SceneId id_ = SceneId::Title;
    SceneId id_next_ = SceneId::Title;
    SceneTransitionRequest scene_trans_req_info_{};

    static void load_default_config(TalkflowControllerT& talkflow);
};

} // namespace app

#endif/*MGC_SCENE_BASE_HPP*/

