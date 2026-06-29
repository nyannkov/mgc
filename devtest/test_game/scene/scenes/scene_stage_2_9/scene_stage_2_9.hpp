#ifndef MGC_SCENE_STAGE_2_9_HPP
#define MGC_SCENE_STAGE_2_9_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_base.hpp"
#include "game_context.hpp"
#include "utils/game_over_effect/game_over_effect.hpp"
#include "entity/player/player.hpp"
#include "entity/stage/stage.hpp"
#include "scene_objects/scene_objects_stage_2_9.hpp"

namespace app {

struct Scene_Stage2_9 : SceneBase {

    explicit Scene_Stage2_9(GameContext& ctx);
    void init() override;
    void update() override;
    void draw(mgc::graphics::Framebuffer& fb) override;

private:
    CameraT camera_;
    stage::Stage stage_;
    SceneObjects_Stage2_9 scene_objects_;
};

} // namespace app

#endif/*MGC_SCENE_STAGE_2_9_HPP*/

