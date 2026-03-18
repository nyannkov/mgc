#ifndef MGC_SCENE_SHOP_HPP
#define MGC_SCENE_SHOP_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_base.hpp"
#include "game_context.hpp"
#include "utils/game_over_effect/game_over_effect.hpp"
#include "entity/player/player.hpp"
#include "entity/stage/stage.hpp"
#include "scene_objects/scene_objects_shop.hpp"
#include "event_objects/event_objects_shop.hpp"

namespace app {

struct Scene_Shop : SceneBase {

    explicit Scene_Shop(GameContext& ctx);
    void init() override;
    void update() override;
    void draw(mgc::graphics::Framebuffer& fb) override;

private:
    CameraT camera_;
    stage::Stage stage_;
    SceneObjects_Shop scene_objects_;
    EventObjects_Shop event_objects_;
};

} // namespace app

#endif/*MGC_SCENE_SHOP_HPP*/

