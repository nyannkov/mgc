#ifndef MGC_SCENE_HPP
#define MGC_SCENE_HPP

#include "scene_base.hpp"
#include "game_context/game_context.hpp"

namespace app {

SceneBase* create_scene(SceneId id, GameContext& ctx);
void destroy_current_scene();

} // namespace app

#endif/*MGC_SCENE_HPP*/

