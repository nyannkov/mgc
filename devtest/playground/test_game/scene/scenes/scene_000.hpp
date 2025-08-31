#ifndef MGC_SCENE_000_HPP
#define MGC_SCENE_000_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_base.hpp"
#include "game_context/game_context.hpp"
#include "entity/player/player.hpp"
#include "entity/enemy/skyfish/skyfish.hpp"
#include "entity/stage/collision_tile_layer/collision_tile_layer.hpp"
#include "entity/stage/tile_layer/tile_layer.hpp"

namespace app {

struct Scene000 : SceneBase {

    explicit Scene000(GameContext& ctx) 
                : id_(SceneId::Id_000),
                  id_next_(SceneId::Id_000),
                  ctx_(ctx),
                  change_request_(false),
                  skyfish_(ctx.frame_timer(), ctx.player()),
                  main_layer_(ctx.frame_timer()),
                  back_layer_(ctx.frame_timer()) {}

    SceneId id() const override { return id_; }
    SceneId id_next() const override { return id_next_; }
    bool has_scene_change_request() const override { return change_request_; };

    void init() override {
        
        main_layer_.set_layer(CollisionTileLayerId::Layer_001);
        main_layer_.set_position({0, 0});

        back_layer_.set_layer(TileLayerId::Layer_001);
        back_layer_.set_position({0, MGC_CELL2PIXEL(34)});
        back_layer_.set_parallax_factor({0.5, 1});

        auto& player = ctx_.player();
        player.set_position({MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(38)});

        skyfish_.set_position({MGC_CELL2PIXEL(6), MGC_CELL2PIXEL(34)});

        camera_.set_target(ctx_.player());
        camera_.set_x_follow_setting(MGC_CELL2PIXEL(6), MGC_CELL2PIXEL(27), MGC_CELL2PIXEL(3));
        camera_.set_y_follow_setting(MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(31), MGC_CELL2PIXEL(3));
        camera_.set_x_follow_enabled(true);
        camera_.set_y_follow_enabled(true);
    }

    void update() override {

        auto& player = ctx_.player();
        
        player.prepare_update();

        skyfish_.prepare_update();

        col_detector_.detect(player, main_layer_);

        mgc::collision::CollisionDetectorBoxToBox::detect(player, skyfish_);

        player.finalize_update();

        skyfish_.finalize_update();

        camera_.update_follow_position();
    }

    void draw(mgc::graphics::Framebuffer& fb) override {

        fb.clear(MGC_COLOR_BLACK);
        
        const auto pos = camera_.follow_position();

        back_layer_.draw(fb, pos);
        main_layer_.draw(fb, pos);
        ctx_.player().draw(fb, pos);
        skyfish_.draw(fb, pos);
    }

private:
   GameContext& ctx_;
   const SceneId id_; 
   SceneId id_next_;
   bool change_request_;
   CollisionTileLayer main_layer_;
   TileLayer back_layer_;
   app::enemy::SkyFish skyfish_;
   mgc::camera::SimpleCameraFollower camera_;
   mgc::collision::CollisionDetectorBoxToMap col_detector_;
};

} // namespace app

#endif/*MGC_SCENE_000_HPP*/

