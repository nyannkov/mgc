#ifndef MGC_SCENE_HELPER_HPP
#define MGC_SCENE_HELPER_HPP

#include "scene_context.hpp"
#include "interface/iscene_objects.hpp"
#include "interface/ievent_objects.hpp"
#include "utils/game_over_effect/game_over_effect.hpp"

namespace app {
namespace scene {

void setup_scene_context(
    SceneContext& scx, 
    stage::Stage* stage,
    ISceneObjects* objs,
    IEventObjects* evts
);

const TalkflowRequest* cutscene_update(
    IEventObjects& cutscenes
); 

void update(
    SceneContext& scx, 
    TalkflowControllerT& talkflow,
    CameraT* camera
);

bool check_scene_transition_request(
    const SceneContext& scx, 
    const GameOverEffect& game_over_effect,
    SceneId& id_next
);

void draw(
    ColorT back_color,
    FramebufferT& fb,
    const CameraT* camera,
    const SceneContext& scx, 
    const TalkflowControllerT& talkflow,
    GameOverEffect& game_over_effect
);

} // namespace scene
} // namespace app

#endif/*MGC_SCENE_HELPER_HPP*/

