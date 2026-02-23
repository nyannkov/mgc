#ifndef MGC_SCENE_FACTORY_HPP
#define MGC_SCENE_FACTORY_HPP

#include "interface/iscene.hpp"
#include "game_context.hpp"

namespace app {

struct SceneFactory {
    static IScene* create_scene(SceneId id, GameContext& ctx);
    static void destroy_current_scene();
};

} // namespace app

#endif/*MGC_SCENE_FACTORY_HPP*/

