#ifndef MGC_SCENE_001_TOWER_FRONT_HPP
#define MGC_SCENE_001_TOWER_FRONT_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_base.hpp"
#include "game_context/game_context.hpp"
#include "utils/game_over_effect/game_over_effect.hpp"
#include "entity/player/player.hpp"
#include "entity/stage/stage.hpp"
#include "scene_objects/scene_001_objects.hpp"
#include "event_objects/event_001_objects.hpp"

namespace app {

struct Scene001_TowerFront : SceneBase {

    explicit Scene001_TowerFront(GameContext& ctx);
    void init() override;
    void update() override;
    void draw(mgc::graphics::Framebuffer& fb) override;

private:
    CameraT camera_;
    stage::Stage stage_;
    Scene001_Objects scene_objects_;
    Event001_Objects event_objects_;
};

} // namespace app

#endif/*MGC_SCENE_001_TOWER_FRONT_HPP*/

