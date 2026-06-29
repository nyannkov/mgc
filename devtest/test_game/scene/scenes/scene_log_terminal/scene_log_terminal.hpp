#ifndef MGC_SCENE_LOG_TERMINAL_HPP
#define MGC_SCENE_LOG_TERMINAL_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_base.hpp"
#include "game_context.hpp"
#include "utils/game_over_effect/game_over_effect.hpp"
#include "entity/player/player.hpp"
#include "entity/stage/stage.hpp"
#include "scene_objects/scene_objects_log_terminal.hpp"
#include "event_objects/event_objects_log_terminal.hpp"

namespace app {

struct Scene_LogTerminal : SceneBase {

    explicit Scene_LogTerminal(GameContext& ctx);
    void init() override;
    void update() override;
    void draw(mgc::graphics::Framebuffer& fb) override;

private:
    CameraT camera_;
    stage::Stage stage_;
    SceneObjects_LogTerminal scene_objects_;
    EventObjects_LogTerminal event_objects_;
};

} // namespace app

#endif/*MGC_SCENE_LOG_TERMINAL_HPP*/

