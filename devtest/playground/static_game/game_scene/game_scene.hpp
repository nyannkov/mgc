#ifndef MGC_GAME_SCENE_HPP
#define MGC_GAME_SCENE_HPP

#include "mgc_cpp/features/updatable.hpp"
#include "game_scene/game_scene_id.hpp"
#include "game_scene/enemy.hpp"
#include "game_scene/enemy_pool.hpp"
#include "game_scene/stage/stage.hpp"
#include "game_scene/scene/scene1.hpp"

namespace app {

struct GameScene {

    GameScene(
        Stage& stage,
        Player& player,
        EnemyPoolT& enemy_pool,
        MainScreenT& main_screen
    ) : stage_(stage),
        player_(player),
        enemy_pool_(enemy_pool),
        main_screen_(main_screen) { }

    void setup(GameSceneId id) {
        id_ = id;

        this->reset_enemy_array();

        if ( id_ == GameSceneId::Scene1 ) {
            scene1::setup(main_screen_, player_, stage_, enemy_pool_);
        }
    }

    GameSceneId scene_id() const {
        return id_;
    }

    void update(GameContext& ctx) {
        if ( id_ == GameSceneId::Scene1 ) {
            scene1::update(ctx, player_, stage_, enemy_pool_);
        }
    }

    void reset_enemy_array() {
        enemy_pool_.destroy_all();
    }

private:
    GameSceneId id_;
    Stage& stage_;
    Player& player_;
    EnemyPoolT& enemy_pool_;
    MainScreenT& main_screen_;
};


}// namespace app


#endif/*MGC_GAME_SCENE_MANAGER_HPP*/

