#ifndef MGC_SCENE_STAGE_3_6_HPP
#define MGC_SCENE_STAGE_3_6_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_base.hpp"
#include "game_context.hpp"
#include "utils/game_over_effect/game_over_effect.hpp"
#include "entity/player/player.hpp"
#include "entity/stage/stage.hpp"
#include "scene_objects/scene_objects_stage_3_6.hpp"
#include "event_objects/event_objects_stage_3_6.hpp"

namespace app {

struct Scene_Stage3_6 : SceneBase {

    explicit Scene_Stage3_6(GameContext& ctx);
    void init() override;
    void update() override;
    void draw(mgc::graphics::Framebuffer& fb) override;

private:
    CameraT camera_;
    stage::Stage stage_;
    SceneObjects_Stage3_6 scene_objects_;
    EventObjects_Stage3_6 event_objects_;
};

} // namespace app

#endif/*MGC_SCENE_STAGE_3_6_HPP*/

