#ifndef MGC_SCENE_MANAGER_HPP
#define MGC_SCENE_MANAGER_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_factory.hpp"
#include "utils/screen_fader/screen_fader.hpp"
#include "menu/equipment_menu.hpp"
#include "game_context.hpp"

namespace app {

struct SceneManager {

    explicit SceneManager(GameContext& ctx) 
        : ctx_(ctx),
          equipment_info_(ctx.world_state.equipment_info),
          scene_info_(ctx.world_state.scene_info),
          menu_(ctx) { }

    void init() {
        set_scene(app::SceneId::Title);
        //set_scene(app::SceneId::Shop);
        //set_scene(app::SceneId::LogTerminal);
        //set_scene(app::SceneId::Stage1_1);
        //set_scene(app::SceneId::Stage1_3);
        //set_scene(app::SceneId::Stage1_4);
        //set_scene(app::SceneId::Stage1_5);
        //set_scene(app::SceneId::Stage1_6);
        //set_scene(app::SceneId::Stage1_7);
        //set_scene(app::SceneId::Stage2_1);
        //set_scene(app::SceneId::Stage2_2);
        //set_scene(app::SceneId::Stage2_3);
        //set_scene(app::SceneId::Stage2_4);
        //set_scene(app::SceneId::Stage2_5);
        //set_scene(app::SceneId::Stage2_6);
        //set_scene(app::SceneId::Stage2_7);
        //set_scene(app::SceneId::Stage2_8);
        //set_scene(app::SceneId::Stage2_9);
        //set_scene(app::SceneId::Stage3_1);
        //set_scene(app::SceneId::Stage3_2);

        equipment_info_.weapon.add(WeaponId::Claw);
        equipment_info_.weapon.equip(WeaponId::Claw);

        //test
        //equipment_info_.weapon.add(WeaponId::Boomerang);
        //equipment_info_.weapon.add(WeaponId::Yoyo);
        //equipment_info_.item.add(ItemId::DivingEquipment);
        //equipment_info_.item.add(ItemId::VioletJewel);
        //ctx_.world_state.player.add_money(1000);
        //ctx_.world_state.checkpoint_info.advance_checkpoint(Checkpoint::WonTheStage2Boss);

        menu_.init();
    }

    void update() {
        if ( !scene_ ) {
            return;
        }

        if ( menu_mode_ ) {
            if ( menu_.is_exit() ) {
                menu_mode_ = false;
            }
        } else {
            if ( scene_->has_menu_request() ) {
                scene_->clear_menu_request();
                menu_.init();
                menu_mode_ = true;
            }
        }

        if ( menu_mode_ ) {
            menu_.update();
        } else {
            if ( update_scene() ) {
                change_next_scene();
            }
        }
    }

    void draw(FramebufferT& fb) {
        if ( !scene_ ) {
            return;
        }

        if ( menu_mode_ ) {
            menu_.draw(fb);
        } else {
            scene_->draw(fb);
            screen_fader_.update(fb);
        }
    }

private:
    IScene *scene_ = nullptr;
    ScreenFader screen_fader_;
    EquipmentInfo& equipment_info_;
    SceneInfo& scene_info_;
    GameContext& ctx_;
    EquipmentMenu menu_;
    bool menu_mode_ = false;

    bool update_scene() {
        bool next_scene_flag = false;
        ScreenFaderState fade_state = screen_fader_.fade_state();

        if ( fade_state == ScreenFaderState::None ) {
            scene_->update();
            if ( scene_->has_scene_change_request() ) {
                screen_fader_.request_fade_out();
            }
        } else {
            if ( fade_state == ScreenFaderState::FadeOutComplete ) {
                screen_fader_.clear();
                next_scene_flag = true; // Change into next scene.
            } else if ( fade_state == ScreenFaderState::FadeInComplete ) {
                screen_fader_.clear();
            } else { }
        }

        return next_scene_flag;
    }

    void change_next_scene() {
        if ( scene_ && scene_->has_scene_change_request() ) {
            scene_info_.set_prev_scene_id(scene_->id());
            set_scene(scene_->id_next());
            screen_fader_.request_fade_in();
        }
    }

    bool set_scene(SceneId id) {
        bool r = true;
        if ( scene_ ) {
            SceneFactory::destroy_current_scene();
        }
        scene_ = SceneFactory::create_scene(id, ctx_);
        if ( scene_ ) {
            scene_->init();
        } else {
            r = false;
        }

        MGC_ASSERT(r, "FAIL: set_scene" );
        return r;
    }
};

} // namespace app

#endif/*MGC_SCENE_MANAGER_HPP*/

