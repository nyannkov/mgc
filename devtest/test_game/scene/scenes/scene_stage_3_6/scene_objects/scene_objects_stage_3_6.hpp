#ifndef MGC_SCENE_OBJECTS_STAGE_3_6_HPP
#define MGC_SCENE_OBJECTS_STAGE_3_6_HPP
#include <utility>
#include "mgc_cpp/mgc.hpp"
#include "scene/scene_context.hpp"
#include "scene/interface/iscene_objects.hpp"
#include "entity/enemy/skyfish/skyfish.hpp"
#include "entity/enemy/walker/walker.hpp"
#include "entity/enemy/balloon/balloon.hpp"
#include "entity/enemy/spectator/spectator.hpp"
#include "entity/enemy/boss/rocker/rocker.hpp"
#include "entity/prop/gate/gate.hpp"
#include "entity/prop/signboard/signboard.hpp"
#include "entity/prop/mic/mic.hpp"
#include "entity/prop/target_balloon/target_balloon.hpp"
#include "utils/triangle/triangle.hpp"

namespace app {

struct Spotlight { 
    
    void turn_on() { triangle_.set_visible(true); }
    void turn_off() { triangle_.set_visible(false); }
    void set_alpha(uint8_t alpha) { alpha_ = alpha; }
    auto alpha() const { return alpha_; }

    void set_position(const mgc::math::Vec2i& pos) { triangle_.set_position(pos); }
    auto position() const { return triangle_.position(); }
    void set_delta1(const mgc::math::Vec2i& delta) { triangle_.set_delta1(delta); }
    void set_delta2(const mgc::math::Vec2i& delta) { triangle_.set_delta2(delta); }
    void set_color(mgc_color_t color) { triangle_.set_color(color); }
    mgc_color_t color() const { return triangle_.color(); }

    bool draw(mgc::graphics::Framebuffer &fb, const mgc::math::Vec2i &cam_pos) const {
        
        mgc::graphics::DrawOptions options = { DRAW_EFFECT_ALPHA_BLEND, alpha_ };
        return triangle_.draw(fb, cam_pos, &options);
    }
    
private:
    mgc::parts::Triangle triangle_{};
    uint8_t alpha_ = 0;
};


struct SceneObjects_Stage3_6 : ISceneObjects {

    explicit SceneObjects_Stage3_6(SceneContext& scx, stage::Stage& stage)
        : cp_info_(scx.world_state.checkpoint_info),
          stage_(stage),
          rocker_(scx.timer, scx.sound),
          walker_ { 
            enemy::Walker(scx.timer, scx.sound),
            enemy::Walker(scx.timer, scx.sound),
            enemy::Walker(scx.timer, scx.sound)
          },
          balloon_ {
            enemy::Balloon(scx.timer, scx.sound, scx.player),
            enemy::Balloon(scx.timer, scx.sound, scx.player)
          },
          skyfish_ {
            enemy::SkyFish(scx.timer, scx.sound, scx.player),
            enemy::SkyFish(scx.timer, scx.sound, scx.player)
          },
          spectator_ {
            enemy::Spectator(scx.timer, scx.sound),// 0
            enemy::Spectator(scx.timer, scx.sound),// 1
            enemy::Spectator(scx.timer, scx.sound),// 2
            enemy::Spectator(scx.timer, scx.sound),// 3
            enemy::Spectator(scx.timer, scx.sound),// 4
            enemy::Spectator(scx.timer, scx.sound),// 5
            enemy::Spectator(scx.timer, scx.sound),// 6
            enemy::Spectator(scx.timer, scx.sound),// 7
            enemy::Spectator(scx.timer, scx.sound),// 8
            enemy::Spectator(scx.timer, scx.sound),// 9
            enemy::Spectator(scx.timer, scx.sound),//10
            enemy::Spectator(scx.timer, scx.sound),//11
            enemy::Spectator(scx.timer, scx.sound),//12
            enemy::Spectator(scx.timer, scx.sound),//13
            enemy::Spectator(scx.timer, scx.sound),//14
            enemy::Spectator(scx.timer, scx.sound),//15
            enemy::Spectator(scx.timer, scx.sound),//16
            enemy::Spectator(scx.timer, scx.sound),//17
            enemy::Spectator(scx.timer, scx.sound),//18
            enemy::Spectator(scx.timer, scx.sound),//19
            enemy::Spectator(scx.timer, scx.sound),//20
            enemy::Spectator(scx.timer, scx.sound),//21
            enemy::Spectator(scx.timer, scx.sound),//22
            enemy::Spectator(scx.timer, scx.sound),//23
          },
          enemies_ { 
            &walker_[0],
            &walker_[1],
            &walker_[2],
            &balloon_[0],
            &balloon_[1],
            &skyfish_[0],
            &skyfish_[1],
            &spectator_[0],
            &spectator_[1],
            &spectator_[2],
            &spectator_[3],
            &spectator_[4],
            &spectator_[5],
            &spectator_[6],
            &spectator_[7],
            &spectator_[8],
            &spectator_[9],
            &spectator_[10],
            &spectator_[11],
            &spectator_[12],
            &spectator_[13],
            &spectator_[14],
            &spectator_[15],
            &spectator_[16],
            &spectator_[17],
            &spectator_[18],
            &spectator_[19],
            &spectator_[20],
            &spectator_[21],
            &spectator_[22],
            &spectator_[23],
            &rocker_
          },
          gate_1_(scx.gamepad),
          gate_2_(scx.gamepad),
          signboard_(scx.gamepad),
          mic_1_(scx.gamepad),
          target_balloons_{
            prop::TargetBalloon(scx.timer),
            prop::TargetBalloon(scx.timer),
            prop::TargetBalloon(scx.timer),
            prop::TargetBalloon(scx.timer),
            prop::TargetBalloon(scx.timer),
            prop::TargetBalloon(scx.timer)
          },
          props_ { 
            &gate_1_,
            &gate_2_,
            &signboard_,
            &mic_1_,
            &target_balloons_[0],
            &target_balloons_[1],
            &target_balloons_[2],
            &target_balloons_[3],
            &target_balloons_[4],
            &target_balloons_[5]
          }
        { }

    void init();

    ArrayViewer<prop::Prop*> props() override { return { props_.data(), props_.size() }; }
    ArrayViewer<item::Item*> items() override { return {}; }
    ArrayViewer<enemy::Enemy*> enemies() override { return { enemies_.data(), enemies_.size() }; }
    ArrayViewer<block::Block*> blocks() override { return {}; }
    ArrayViewer<civilian::Civilian*> civils() override { return {}; }
    ArrayViewer<carrier::Carrier*> carriers() override { return {}; }

    auto& walker() { return walker_; }
    auto& skyfish() { return skyfish_; }
    auto& balloon() { return balloon_; }
    auto& rocker() { return rocker_; }
    auto& stage() { return stage_; }
    auto& spotlight1() { return spotlight1_; }
    auto& spotlight2() { return spotlight2_; }
    auto& spectator() { return spectator_; }
    auto& target_balloons() { return target_balloons_; }

    void all_enemy_despawn() {
        for ( auto* enemy : enemies() ) {
            if ( enemy->enemy_state() == enemy::EnemyState::Active ) {
                enemy->despawn();
            }
        }
    }

    bool is_all_enemy_despawned() {
        for ( auto* enemy : this->enemies() ) {
            if ( enemy->enemy_state() != enemy::EnemyState::Inactive ) {
                return false;
            }
        }
        return true;
    }

    void draw(
        FramebufferT& fb,
        mgc::math::Vec2i& cam_pos
    ) override {
        for ( auto* carrier : carriers() ) {
            carrier->draw(fb, cam_pos);
        }
        for ( auto* civil : civils() ) {
            civil->draw(fb, cam_pos);
        }
        for ( auto* prop : props() ) {
            if ( prop != &mic_1_ ) {
                prop->draw(fb, cam_pos);
            }
        }
        for ( auto* enemy : enemies() ) {
            enemy->draw(fb, cam_pos);
            for ( auto* weapon : enemy->weapons() ) {
                weapon->draw(fb, cam_pos);
            }
        }
        for ( auto* item : items() ) {
            item->draw(fb, cam_pos);
        }
        for ( auto* block : blocks() ) {
            block->draw(fb, cam_pos);
        }
    }


    void draw_after (
        FramebufferT& fb,
        mgc::math::Vec2i& cam_pos
    ) override { 

        mic_1_.draw(fb, cam_pos);
        spotlight1_.draw(fb, cam_pos);
        spotlight2_.draw(fb, cam_pos);
    }

private:
    CheckpointInfo& cp_info_;
    stage::Stage& stage_;

    // Enemy
    std::array<enemy::Walker, 3> walker_;
    std::array<enemy::Balloon, 2> balloon_;
    std::array<enemy::SkyFish, 2> skyfish_;
    std::array<enemy::Spectator, 24> spectator_;
    enemy::Rocker rocker_;
    std::array<enemy::Enemy*, 32> enemies_;

    // Prop
    prop::Gate gate_1_;
    prop::Gate gate_2_;
    prop::Mic mic_1_;
    prop::Signboard signboard_;
    std::array<prop::TargetBalloon, 6>  target_balloons_;
    std::array<prop::Prop*, 10> props_;

    Spotlight spotlight1_;
    Spotlight spotlight2_;
};

} // namespace app

#endif // MGC_SCENE_OBJECTS_STAGE_3_6_HPP

