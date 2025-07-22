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
    SkyFish1,
    SkyFish2,
    SkyFish3,
    SkyFish4,
};
    
void setup(
    MainScreenT& main_screen,
    Player& player,
    Stage& stage,
    EnemyPoolT& enemy_pool
) {

    stage.setup_by_id(app::Stage::StageId::Stage1);
    stage.set_position(mgc::math::Vec2i(0, 0));

    Enemy* enemy1 = enemy_pool.spawn(static_cast<mgc_id_t>(EnemyEntityId::SkyFish1), EnemyKind::SkyFish);

    Enemy* enemy2 = enemy_pool.spawn(static_cast<mgc_id_t>(EnemyEntityId::SkyFish2), EnemyKind::SkyFish);
    enemy2->set_position(mgc::math::Vec2i(MGC_CELL2PIXEL(9), MGC_CELL2PIXEL(33)));

    Enemy* enemy3 = enemy_pool.spawn(static_cast<mgc_id_t>(EnemyEntityId::SkyFish3), EnemyKind::SkyFish);
    enemy3->set_position(mgc::math::Vec2i(MGC_CELL2PIXEL(11), MGC_CELL2PIXEL(32)));

    Enemy* enemy4 = enemy_pool.spawn(static_cast<mgc_id_t>(EnemyEntityId::SkyFish4), EnemyKind::SkyFish);
    enemy4->set_position(mgc::math::Vec2i(MGC_CELL2PIXEL(8), MGC_CELL2PIXEL(30)));

    auto& camera = main_screen.camera();
    camera.set_target(player);
    camera.set_x_follow_setting(MGC_CELL2PIXEL(6), MGC_CELL2PIXEL(27), MGC_CELL2PIXEL(3));
    camera.set_y_follow_setting(MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(31), MGC_CELL2PIXEL(3));
    camera.set_x_follow_enabled(true);
    camera.set_y_follow_enabled(true);

    // TODO scope
    main_screen.register_drawable(stage.bg_layer());
    main_screen.register_drawable(stage.main_layer());
    main_screen.register_drawable(enemy1);
    main_screen.register_drawable(enemy2);
    main_screen.register_drawable(enemy3);
    main_screen.register_drawable(enemy4);
    main_screen.register_drawable(&player);
}

void update(
    GameContext& ctx, 
    Player& player,
    Stage& stage,
    EnemyPoolT& enemy_pool
) {
    stage.update();

    Enemy* enemy1 = enemy_pool.get(static_cast<mgc_id_t>(EnemyEntityId::SkyFish1));
    Enemy* enemy2 = enemy_pool.get(static_cast<mgc_id_t>(EnemyEntityId::SkyFish2));
    Enemy* enemy3 = enemy_pool.get(static_cast<mgc_id_t>(EnemyEntityId::SkyFish3));
    Enemy* enemy4 = enemy_pool.get(static_cast<mgc_id_t>(EnemyEntityId::SkyFish4));
    enemy1->update(ctx);
    enemy2->update(ctx);
    enemy3->update(ctx);
    enemy4->update(ctx);

    player.update(ctx);
}

}// namespace scene1
}// namespace app


#endif/*MGC_SCENE1_HPP*/

