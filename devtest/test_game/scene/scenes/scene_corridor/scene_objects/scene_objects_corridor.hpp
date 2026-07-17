#ifndef MGC_SCENE_OBJECTS_CORRIDOR_HPP
#define MGC_SCENE_OBJECTS_CORRIDOR_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_context.hpp"
#include "scene/interface/iscene_objects.hpp"
#include "entity/prop/entrance/entrance.hpp"
#include "entity/prop/signboard/signboard.hpp"
#include "entity/prop/gate/gate.hpp"
#include "resources/generated/talkscript/talkscript_1.h"
#include "resources/generated/tileset/tileset_light_rays.h"

namespace app {
struct SceneObjects_Corridor : ISceneObjects {

    explicit SceneObjects_Corridor(SceneContext& scx)
        : cp_info_(scx.world_state.checkpoint_info),
          signboard_(scx.gamepad),
          gate_(scx.gamepad),
          props_ { &entrance_, &signboard_, &gate_ } {
    }

    void init() override {
        entrance_.spawn({MGC_CELL2PIXEL(3)-6, MGC_CELL2PIXEL(8)}, prop::EntranceType::Type2);
        entrance_.set_scene_transition_request(
            { SceneId::TowerFront }
        );

        signboard_.spawn({MGC_CELL2PIXEL(35), MGC_CELL2PIXEL(10)});
        signboard_.set_talkflow_request({
            &talkscript_1,
            MGC_TALKSCRIPT_1_SIGNBOARD_1_2,
            nullptr,
            TalkflowEffectType::Mute
        });

        gate_.spawn({MGC_CELL2PIXEL(37), MGC_CELL2PIXEL(10)}, false);
        gate_.set_scene_transition_request(
            { SceneId::Stage1_1 }
        );
        if ( cp_info_.has_reached(Checkpoint::Corridor_OpenedTheDoor) ) {
            gate_.open();
        } else {
            gate_.close();
            gate_.set_talkflow_request({
                &talkscript_1,
                MGC_TALKSCRIPT_1_GATE_1,
                nullptr,
                TalkflowEffectType::Mute
            });
        }

        light_rays_[0].set_visible(true);
        light_rays_[0].set_tileset(tileset_light_rays);
        light_rays_[0].set_tile_index(1);
        light_rays_[0].set_position({MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(8)});

        light_rays_[1].set_visible(true);
        light_rays_[1].set_tileset(tileset_light_rays);
        light_rays_[1].set_tile_index(2);
        light_rays_[1].set_position({MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(9)});

        light_rays_[2].set_visible(true);
        light_rays_[2].set_tileset(tileset_light_rays);
        light_rays_[2].set_tile_index(0);
        light_rays_[2].set_position({MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(10)});

        light_rays_[3].set_visible(true);
        light_rays_[3].set_tileset(tileset_light_rays);
        light_rays_[3].set_tile_index(1);
        light_rays_[3].set_position({MGC_CELL2PIXEL(4), MGC_CELL2PIXEL(10)});
    }

    ArrayViewer<prop::Prop*> props() override {
        return { props_.data(), props_.size() };
    }
    ArrayViewer<civilian::Civilian*> civils() override { return {}; }
    ArrayViewer<enemy::Enemy*> enemies() override { return {}; }
    ArrayViewer<item::Item*> items() override { return {}; }
    ArrayViewer<block::Block*> blocks() override { return {}; }
    ArrayViewer<carrier::Carrier*> carriers() override { return {}; }

    void draw_after (
        FramebufferT& fb,
        mgc::math::Vec2i& cam_pos
    ) override { 
        mgc::graphics::DrawOptions options = { DRAW_EFFECT_ALPHA_BLEND, 10 };

        for ( auto& obj : light_rays_ ) {
            obj.draw(fb, cam_pos, &options);
        }
    }

    auto& entrance() { return entrance_; }
    auto& signboard() { return signboard_; }
    auto& gate() { return gate_; }

private:
    CheckpointInfo& cp_info_;
    prop::Entrance entrance_;
    prop::Signboard signboard_;
    prop::Gate gate_;
    std::array<prop::Prop*, 3> props_;
    SpriteT light_rays_[4];// TODO map layer
};

} // namespace app

#endif // MGC_SCENE_OBJECTS_CORRIDOR_HPP

