#include "scene.hpp"
#include "scene_pool.hpp"
#include "scenes/scene_000_title/scene_000.hpp"
#include "scenes/scene_001_tower_front/scene_001_tower_front.hpp"
#include "scenes/scene_002_corridor/scene_002_corridor.hpp"
#include "scenes/scene_003_stage_1_1/scene_003_stage_1_1.hpp"
#include "scenes/scene_999_game_over/scene_999_game_over.hpp"

namespace app {

ScenePool pool;

IScene* create_scene(SceneId id, GameContext& ctx) {
    switch ( id ) {
    case SceneId::Id_000:
        pool.create<Scene000>(ctx);
        break;
    case SceneId::Id_001_TowerFront:
        pool.create<Scene001_TowerFront>(ctx);
        break;
    case SceneId::Id_002_Corridor:
        pool.create<Scene002_Corridor>(ctx);
        break;
    case SceneId::Id_003_Stage1_1:
        pool.create<Scene003_Stage1_1>(ctx);
        break;
    case SceneId::Id_999_GameOver:
        pool.create<Scene999_GameOver>(ctx);
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

