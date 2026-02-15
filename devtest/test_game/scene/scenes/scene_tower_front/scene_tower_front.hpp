#ifndef MGC_SCENE_TOWER_FRONT_HPP
#define MGC_SCENE_TOWER_FRONT_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_base.hpp"
#include "game_context/game_context.hpp"
#include "utils/game_over_effect/game_over_effect.hpp"
#include "entity/player/player.hpp"
#include "entity/stage/stage.hpp"
#include "scene_objects/scene_objects_tower_front.hpp"
#include "event_objects/event_objects_tower_front.hpp"

namespace app {

struct Scene_TowerFront : SceneBase {

    explicit Scene_TowerFront(GameContext& ctx);
    void init() override;
    void update() override;
    void draw(mgc::graphics::Framebuffer& fb) override;

private:
    CameraT camera_;
    stage::Stage stage_;
    SceneObjects_TowerFront scene_objects_;
    EventObjects_TowerFront event_objects_;
};

} // namespace app

#endif/*MGC_SCENE_001_TOWER_FRONT_HPP*/

