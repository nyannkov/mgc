#ifndef MGC_SCENE1_HPP
#define MGC_SCENE1_HPP

#include "mgc_cpp/features/updatable.hpp"
#include "app_typedefs.hpp"
#include "game_scene/scene/game_scene_id.hpp"
#include "game_scene/player/player.hpp"
#include "game_scene/enemy/enemy_pool.hpp"
#include "game_scene/stage/stage.hpp"

namespace app {
namespace scene1 {

enum class EnemyEntityId : mgc_id_t {
    SkyFish1
};
    
void setup(
    MainScreenT& main_screen,
    Player& player,
    Stage& stage,
    EnemyPoolT& enemy_pool
) {

    stage.setup_by_id(app::Stage::StageId::Stage1);
    stage.set_position(mgc::math::Vec2i(0, 0));

    Enemy* enemy = enemy_pool.spawn(static_cast<mgc_id_t>(EnemyEntityId::SkyFish1), EnemyKind::SkyFish);

    auto& camera = main_screen.camera();
    camera.set_target(player);
    camera.set_x_follow_setting(MGC_CELL2PIXEL(6), MGC_CELL2PIXEL(27), MGC_CELL2PIXEL(3));
    camera.set_y_follow_setting(MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(31), MGC_CELL2PIXEL(3));
    camera.set_x_follow_enabled(true);
    camera.set_y_follow_enabled(true);

    // TODO scope
    main_screen.register_drawable(stage.bg_layer());
    main_screen.register_drawable(stage.main_layer());
    main_screen.register_drawable(enemy);
    main_screen.register_drawable(&player);
}

void update(
    GameContext& ctx, 
    Player& player,
    Stage& stage,
    EnemyPoolT& enemy_pool
) {
    stage.update();

    Enemy* enemy = enemy_pool.get(static_cast<mgc_id_t>(EnemyEntityId::SkyFish1));
    enemy->update(ctx);

    player.update(ctx);
}

}// namespace scene1
}// namespace app


#endif/*MGC_SCENE1_HPP*/

