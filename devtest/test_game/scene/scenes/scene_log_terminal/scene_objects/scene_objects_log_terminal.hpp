#ifndef MGC_SCENE_OBJECTS_LOG_TERMINAL_HPP
#define MGC_SCENE_OBJECTS_LOG_TERMINAL_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_context.hpp"
#include "scene/interface/iscene_objects.hpp"
#include "entity/prop/entrance/entrance.hpp"
#include "entity/prop/signboard/signboard.hpp"
#include "entity/prop/jewel/jewel.hpp"
#include "entity/prop/gate/gate.hpp"
#include "resources/generated/talkscript/talkscript_1.h"
#include "resources/generated/tileset/tileset_static_objects_48x24.h"

namespace app {

struct Terminal {

    void spawn(mgc::math::Vec2i pos) {
        is_turn_on_terminal_ = false;
        alpha_ = ALPHA_TERM_OFF_;
        terminal_.set_visible(true);
        terminal_.set_tileset(tileset_static_objects_48x24);
        terminal_.set_tile_index(0);
        terminal_.set_position(pos);
    }

    bool draw(
        mgc::graphics::Framebuffer &fb,
        const mgc::math::Vec2i &cam_pos
    ) {
        mgc::graphics::DrawOptions options = { DRAW_EFFECT_ALPHA_BLEND, 140 };
        if ( is_turn_on_terminal_ ) {
            if ( ( rand() % 100) < 3 ) {
                if ( alpha_ == ALPHA_TERM_ON_BRIGHT ) {
                    alpha_ =ALPHA_TERM_ON_;
                    terminal_.set_tile_index(2);
                } else {
                    alpha_ = ALPHA_TERM_ON_BRIGHT;
                    terminal_.set_tile_index(1);
                }
            }
        } else {
            alpha_ = ALPHA_TERM_OFF_;
            terminal_.set_tile_index(0);
        }
        return terminal_.draw(fb, cam_pos, &options);
    }

    void turn_on() {
        is_turn_on_terminal_ = true;
        alpha_ = ALPHA_TERM_ON_BRIGHT;
        terminal_.set_tile_index(1);
    }

    void turn_off() {
        is_turn_on_terminal_ = false;
        alpha_ = ALPHA_TERM_OFF_;
        terminal_.set_tile_index(0);
    }

    uint8_t alpha() const { return alpha_; }

private:
    static constexpr uint8_t ALPHA_TERM_OFF_ = 32;
    static constexpr uint8_t ALPHA_TERM_ON_ = 100;
    static constexpr uint8_t ALPHA_TERM_ON_BRIGHT = 150;
    SpriteT terminal_;
    uint8_t alpha_ = ALPHA_TERM_OFF_;
    bool is_turn_on_terminal_ = false;
};

struct SceneObjects_LogTerminal : ISceneObjects {

    explicit SceneObjects_LogTerminal(SceneContext& scx)
        : cp_info_(scx.world_state.checkpoint_info),
          signboard_(scx.gamepad),
          prev_id_(scx.world_state.scene_info.prev_scene_id()),
          gate_(scx.gamepad),
          jewel_(scx.gamepad, scx.timer),
          props_ { &signboard_, &gate_, &jewel_ } {
    }

    void init() override {
        gate_.spawn({MGC_CELL2PIXEL(22), MGC_CELL2PIXEL(10)});
        gate_.set_gate_type(prop::GateType::Type2);
        gate_.open();
        gate_.set_scene_transition_request(
            { prev_id_ }
        );
        
        terminal_.spawn({MGC_CELL2PIXEL(12), MGC_CELL2PIXEL(7)});
    }

    ArrayViewer<prop::Prop*> props() override {
        return { props_.data(), props_.size() };
    }
    ArrayViewer<civilian::Civilian*> civils() override { return {}; }
    ArrayViewer<enemy::Enemy*> enemies() override { return {}; }
    ArrayViewer<item::Item*> items() override { return {}; }
    ArrayViewer<block::Block*> blocks() override { return {}; }
    ArrayViewer<carrier::Carrier*> carriers() override { return {}; }

    void draw(
        FramebufferT& fb,
        mgc::math::Vec2i& cam_pos
    ) override {
        ISceneObjects::draw(fb, cam_pos);
        terminal_.draw(fb, cam_pos);
    }

    void draw_after (
        FramebufferT& fb,
        mgc::math::Vec2i& cam_pos
    ) override { 

        fill_color_with_alpha(fb, MGC_COLOR_BLACK, terminal_.alpha());
    }

    auto& signboard() { return signboard_; }
    auto& gate() { return gate_; }
    auto& jewel() { return jewel_; }
    auto& terminal() { return terminal_; }

private:
    CheckpointInfo& cp_info_;
    const SceneId prev_id_;
    prop::Signboard signboard_;
    prop::Gate gate_;
    prop::Jewel jewel_;
    std::array<prop::Prop*, 3> props_;
    Terminal terminal_;
};

} // namespace app

#endif // MGC_SCENE_OBJECTS_LOG_TERMINAL_HPP

