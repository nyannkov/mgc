#ifndef MGC_SCENE_002_CORRIDOR_HPP
#define MGC_SCENE_002_CORRIDOR_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_base.hpp"
#include "game_context/game_context.hpp"
#include "utils/game_over_effect/game_over_effect.hpp"
#include "entity/player/player.hpp"
#include "entity/stage/stage.hpp"
#include "scene_objects/scene_002_objects.hpp"
#include "event_objects/event_002_objects.hpp"

namespace app {

struct Scene002_Corridor : SceneBase {

    explicit Scene002_Corridor(GameContext& ctx);
    void init() override;
    void update() override;
    void draw(mgc::graphics::Framebuffer& fb) override;

private:
    CameraT camera_;
    stage::Stage stage_;
    Scene002_Objects scene_objects_;
    Event002_Objects event_objects_;
};

} // namespace app

#endif/*MGC_SCENE_002_CORRIDOR_HPP*/

