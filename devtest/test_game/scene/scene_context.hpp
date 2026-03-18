#ifndef MGC_SCENE_CONTEXT_HPP
#define MGC_SCENE_CONTEXT_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "game_context.hpp"
#include "talkflow_effects.hpp"
#include "entity/stage/stage.hpp"
#include "interface/iscene_objects.hpp"
#include "interface/ievent_objects.hpp"

namespace app {

struct SceneContext {
    SoundControllerT& sound;
    const GamepadT& gamepad;
    const FrameTimerT& timer;
    Player& player;
    TalkflowEffects& talkflow_effects;
    WorldState& world_state;
    stage::Stage* stage;
    ISceneObjects* objs;
    IEventObjects* evts;
};

} // namespace app

#endif/*MGC_SCENE_CONTEXT*/

