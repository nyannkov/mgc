#include "scene_factory.hpp"
#include "scene_pool.hpp"
#include "scenes/scene_title/scene_title.hpp"
#include "scenes/scene_password/scene_password.hpp"
#include "scenes/scene_tower_front/scene_tower_front.hpp"
#include "scenes/scene_corridor/scene_corridor.hpp"
#include "scenes/scene_stage_1_1/scene_stage_1_1.hpp"
#include "scenes/scene_stage_1_2/scene_stage_1_2.hpp"
#include "scenes/scene_stage_1_3/scene_stage_1_3.hpp"
#include "scenes/scene_stage_1_4/scene_stage_1_4.hpp"
#include "scenes/scene_stage_1_5/scene_stage_1_5.hpp"
#include "scenes/scene_stage_1_6/scene_stage_1_6.hpp"
#include "scenes/scene_stage_1_7/scene_stage_1_7.hpp"
#include "scenes/scene_stage_2_1/scene_stage_2_1.hpp"
#include "scenes/scene_stage_2_2/scene_stage_2_2.hpp"
#include "scenes/scene_stage_2_3/scene_stage_2_3.hpp"
#include "scenes/scene_stage_2_4/scene_stage_2_4.hpp"
#include "scenes/scene_stage_2_5/scene_stage_2_5.hpp"
#include "scenes/scene_stage_2_6/scene_stage_2_6.hpp"
#include "scenes/scene_stage_2_7/scene_stage_2_7.hpp"
#include "scenes/scene_stage_2_8/scene_stage_2_8.hpp"
#include "scenes/scene_shop/scene_shop.hpp"
#include "scenes/scene_game_over/scene_game_over.hpp"

namespace app {
namespace {
ScenePool pool;
}

IScene* SceneFactory::create_scene(SceneId id, GameContext& ctx) {
    switch ( id ) {
    case SceneId::Title:
        pool.create<Scene_Title>(ctx);
        break;
    case SceneId::Password:
        pool.create<Scene_Password>(ctx);
        break;
    case SceneId::TowerFront:
        pool.create<Scene_TowerFront>(ctx);
        break;
    case SceneId::Corridor:
        pool.create<Scene_Corridor>(ctx);
        break;
    case SceneId::Stage1_1:
        pool.create<Scene_Stage1_1>(ctx);
        break;
    case SceneId::Stage1_2:
        pool.create<Scene_Stage1_2>(ctx);
        break;
    case SceneId::Stage1_3:
        pool.create<Scene_Stage1_3>(ctx);
        break;
    case SceneId::Stage1_4:
        pool.create<Scene_Stage1_4>(ctx);
        break;
    case SceneId::Stage1_5:
        pool.create<Scene_Stage1_5>(ctx);
        break;
    case SceneId::Stage1_6:
        pool.create<Scene_Stage1_6>(ctx);
        break;
    case SceneId::Stage1_7:
        pool.create<Scene_Stage1_7>(ctx);
        break;
    case SceneId::Stage2_1:
        pool.create<Scene_Stage2_1>(ctx);
        break;
    case SceneId::Stage2_2:
        pool.create<Scene_Stage2_2>(ctx);
        break;
    case SceneId::Stage2_3:
        pool.create<Scene_Stage2_3>(ctx);
        break;
    case SceneId::Stage2_4:
        pool.create<Scene_Stage2_4>(ctx);
        break;
    case SceneId::Stage2_5:
        pool.create<Scene_Stage2_5>(ctx);
        break;
    case SceneId::Stage2_6:
        pool.create<Scene_Stage2_6>(ctx);
        break;
    case SceneId::Stage2_7:
        pool.create<Scene_Stage2_7>(ctx);
        break;
    case SceneId::Stage2_8:
        pool.create<Scene_Stage2_8>(ctx);
        break;
    case SceneId::Shop:
        pool.create<Scene_Shop>(ctx);
        break;
    case SceneId::GameOver:
        pool.create<Scene_GameOver>(ctx);
        break;
    default:
        return nullptr;
    }

    return pool.get();
}

void SceneFactory::destroy_current_scene() {
    pool.destroy();
}

} // namespace app

