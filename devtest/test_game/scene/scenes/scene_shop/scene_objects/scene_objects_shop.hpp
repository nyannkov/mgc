#ifndef MGC_SCENE_OBJECTS_SHOP_HPP
#define MGC_SCENE_OBJECTS_SHOP_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_context.hpp"
#include "scene/interface/iscene_objects.hpp"
#include "entity/prop/door/door.hpp"
#include "entity/prop/portal/portal.hpp"
#include "entity/civilian/florist/florist.hpp"
#include "resources/generated/talkscript/talkscript_1.h"
#include "resources/generated/tileset/tileset_shop_elements.h"
#include "resources/mml/mml.h"

namespace app {

struct SceneObjects_Shop : ISceneObjects {

    explicit SceneObjects_Shop(SceneContext& scx)
        : cp_info_(scx.world_state.checkpoint_info),
          door_(scx.gamepad),
          portal_(scx.gamepad, scx.timer),
          props_ { &door_, &portal_ },
          florist_(
            scx.gamepad, 
            scx.world_state,
            scx.player
          ),
          civils_ {&florist_ } {

          init();
    }

    void init() override {
        door_.spawn({MGC_CELL2PIXEL(16), MGC_CELL2PIXEL(5)+8}, false);
        door_.lock();
        door_.set_talkflow_request({
            &talkscript_1,
            MGC_TALKSCRIPT_1_GATE_1,
            nullptr,
            TalkflowEffectType::Mute
        });

        portal_.spawn({MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(5)+8});

        florist_.spawn(
            {MGC_CELL2PIXEL(10), MGC_CELL2PIXEL(5)+8},
            civilian::FloristAnimState::Stand_Left);


        table_left_.set_tileset(tileset_shop_elements);
        table_left_.set_tile_index(25);
        table_left_.set_position({MGC_CELL2PIXEL(8), MGC_CELL2PIXEL(6)});

        table_right_.set_tileset(tileset_shop_elements);
        table_right_.set_tile_index(26);
        table_right_.set_position({MGC_CELL2PIXEL(9), MGC_CELL2PIXEL(6)});

        chair_left_.set_tileset(tileset_shop_elements);
        chair_left_.set_tile_index(35);
        chair_left_.set_position({MGC_CELL2PIXEL(8), MGC_CELL2PIXEL(6)});

        chair_right_.set_tileset(tileset_shop_elements);
        chair_right_.set_tile_index(36);
        chair_right_.set_position({MGC_CELL2PIXEL(9), MGC_CELL2PIXEL(6)});
    }

    ArrayViewer<prop::Prop*> props() override {
        return { props_.data(), props_.size() };
    }
    ArrayViewer<civilian::Civilian*> civils() override {
        return { civils_.data(), civils_.size() };
    }
    ArrayViewer<enemy::Enemy*> enemies() override { return {}; }
    ArrayViewer<item::Item*> items() override { return {}; }
    ArrayViewer<block::Block*> blocks() override { return {}; }

    auto& door() { return door_; }
    auto& portal() { return portal_; }
    auto& chair_left() { return chair_left_; }
    auto& florist() { return florist_; }

    void set_table_layer(bool is_front) {
        table_front_ = is_front;
    }

    void draw(
        FramebufferT& fb,
        mgc::math::Vec2i& cam_pos
    ) override {

        ISceneObjects::draw(fb, cam_pos);
        chair_left_.draw(fb, cam_pos);
        chair_right_.draw(fb, cam_pos);
        if ( !table_front_ ) {
            table_left_.draw(fb, cam_pos);
            table_right_.draw(fb, cam_pos);
        }
    }

    void draw_after(
        FramebufferT& fb,
        mgc::math::Vec2i& cam_pos
    ) override {
        if ( table_front_ ) {
            table_left_.draw(fb, cam_pos);
            table_right_.draw(fb, cam_pos);
        }
    }

private:
    CheckpointInfo& cp_info_;
    prop::Door door_;
    prop::Portal portal_;
    std::array<prop::Prop*, 2> props_;
    civilian::Florist florist_;
    std::array<civilian::Civilian*, 1> civils_;
    SpriteT table_left_;
    SpriteT table_right_;
    SpriteT chair_left_;
    SpriteT chair_right_;
    bool table_front_ = false;
};

} // namespace app

#endif // MGC_SCENE_OBJECTS_SHOP_HPP

