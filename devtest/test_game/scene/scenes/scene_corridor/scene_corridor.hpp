#ifndef MGC_SCENE_CORRIDOR_HPP
#define MGC_SCENE_CORRIDOR_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_base.hpp"
#include "game_context.hpp"
#include "utils/game_over_effect/game_over_effect.hpp"
#include "entity/player/player.hpp"
#include "entity/stage/stage.hpp"
#include "scene_objects/scene_objects_corridor.hpp"
#include "event_objects/event_objects_corridor.hpp"

namespace app {

struct Scene_Corridor : SceneBase {

    explicit Scene_Corridor(GameContext& ctx);
    void init() override;
    void update() override;
    void draw(mgc::graphics::Framebuffer& fb) override;

private:
    CameraT camera_;
    stage::Stage stage_;
    SceneObjects_Corridor scene_objects_;
    EventObjects_Corridor event_objects_;
};

} // namespace app

#endif/*MGC_SCENE_CORRIDOR_HPP*/

