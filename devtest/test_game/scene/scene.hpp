#ifndef MGC_SCENE_HPP
#define MGC_SCENE_HPP

#include "interface/iscene.hpp"
#include "game_context/game_context.hpp"

namespace app {

IScene* create_scene(SceneId id, GameContext& ctx);
void destroy_current_scene();

} // namespace app

#endif/*MGC_SCENE_HPP*/

