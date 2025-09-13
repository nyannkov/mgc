#ifndef MGC_SCENE_001_HPP
#define MGC_SCENE_001_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_base.hpp"
#include "game_context/game_context.hpp"
#include "utils/screen_fader/screen_fader.hpp"
#include "entity/player/player.hpp"
#include "entity/stage/collision_tile_layer/collision_tile_layer.hpp"
#include "entity/stage/tile_layer/tile_layer.hpp"
#include "enemies/scene_001_enemies.hpp"
#include "items/scene_001_items.hpp"
#include "props/scene_001_props.hpp"

namespace app {

struct Scene001 : SceneBase {

    explicit Scene001(GameContext& ctx);

    SceneId id() const override { return id_; }
    SceneId id_next() const override { return id_next_; }
    bool has_scene_change_request() const override { return change_request_; };

    void init() override;
    void update() override;
    void draw(mgc::graphics::Framebuffer& fb) override;

private:
    const FrameTimerT& frame_timer_;
    SoundControllerT& sound_controller_;
    Player& player_;
    StatusDisplayRequest& status_display_request_;
    StageInfo& stage_info_;
    const SceneId id_; 
    SceneId id_next_;
    bool change_request_;
    CollisionTileLayer main_layer_;
    TileLayer back1_layer_;
    TileLayer back2_layer_;
    mgc::camera::SimpleCameraFollower camera_;
    mgc::collision::CollisionDetectorBoxToMap col_detector_;
    Scene001_Enemies enemies_;
    Scene001_Items items_;
    Scene001_Props props_;
    ScreenFader screen_fader_;
    TalkflowControllerT talkflow_controller_;
    // TODO
    struct TalkflowListener : ITalkflowListenerT {
        explicit TalkflowListener(Player& player) 
            : player_(player) { }
        void on_choice_done(
            mgc_node_idx_t tag,
            size_t item_tag,
            int32_t value
        ) override; 
        void on_flow_end(mgc_node_idx_t tag) override { }
        bool continue_requested() const { return continue_requested_; }
    private:
        Player& player_;
        bool continue_requested_ = false;
    } talkflow_listener_;

};

} // namespace app

#endif/*MGC_SCENE_001_HPP*/

