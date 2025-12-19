#ifndef MGC_SCENE_003_STAGE_1_1_HPP
#define MGC_SCENE_003_STAGE_1_1_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_base.hpp"
#include "game_context/game_context.hpp"
#include "utils/game_over_effect/game_over_effect.hpp"
#include "entity/player/player.hpp"
#include "entity/stage/stage.hpp"
#include "scene_objects/scene_003_objects.hpp"

namespace app {

struct Scene003_Stage1_1 : SceneBase {

    explicit Scene003_Stage1_1(GameContext& ctx);
    void init() override;
    void update() override;
    void draw(mgc::graphics::Framebuffer& fb) override;

private:
    CameraT camera_;
    stage::Stage stage_;
    Scene003_Objects scene_objects_;
    ScreenFader screen_fader_;
};

} // namespace app

#endif/*MGC_SCENE_003_STAGE_1_1_HPP*/

