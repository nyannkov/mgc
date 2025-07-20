#ifndef MGC_GAME_SCENE_MANAGER_HPP
#define MGC_GAME_SCENE_MANAGER_HPP

#include "mgc_cpp/features/updatable.hpp"
#include "app_typedefs.hpp"
#include "game_scene/game_scene.hpp"
#include "game_scene/game_scene_id.hpp"
#include "game_scene/stage/stage.hpp"
#include "game_scene/player.hpp"
#include "game_scene/enemy.hpp"
#include "game_scene/enemy_pool.hpp"

namespace app {

struct GameSceneManager {

    explicit GameSceneManager(PlatformT& platform) 
        : platform_(platform),
          status_screen_(platform.display_impl()),
          main_screen_(platform.display_impl()),
          game_context_(platform.input(), platform.sound(), player_.position(), player_.life_value()),
          game_scene_(stage_, player_, enemy_pool_, main_screen_) { }

    void set_scene(GameSceneId id) {
        game_scene_.setup(id);
    }

    void run() {
        
        platform_.input_impl().proc();

        game_scene_.update(game_context_);

        collision_detector_b2m_.detect(player_, stage_, mgc::collision::CollisionDetectorBoxToMap::CornerPushDirection::PreferX);
        for ( auto& slot : enemy_pool_.enemy_slots ) {
            if ( slot.active ) {
                mgc::collision::CollisionDetectorBoxToBox::detect(player_, slot.enemy);
            }
        }

        status_screen_.update(game_context_);

        main_screen_.camera().update_follow_position();

        main_screen_.draw_to_buffer();
        main_screen_.sync();
        status_screen_.draw_blocking_if_needed();
        main_screen_.flush();
    }

private:
    PlatformT& platform_;
    StatusScreenT status_screen_;
    MainScreenT main_screen_;
    Stage stage_;
    Player player_;
    EnemyPoolT enemy_pool_;
    GameContext game_context_;
    GameScene game_scene_;
    mgc::collision::CollisionDetectorBoxToMap collision_detector_b2m_;
};


}// namespace app


#endif/*MGC_GAME_SCENE_MANAGER_HPP*/

