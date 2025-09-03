#include "scene.hpp"
#include "scene_pool.hpp"
#include "scenes/scene_000.hpp"
#include "scenes/scene_001.hpp"

namespace app {

ScenePool pool;

SceneBase* create_scene(SceneId id, GameContext& ctx) {
    switch ( id ) {
    case SceneId::Id_000:
        pool.create<Scene000>(ctx);
        break;
    case SceneId::Id_001:
        pool.create<Scene001>(ctx);
        break;
    default:
        return nullptr;
    }

    return pool.get();
}

void destroy_current_scene() {
    pool.destroy();
}

} // namespace app

