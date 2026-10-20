#include <math.h>
#include "entity/player/player.hpp"
#include "target_balloon.hpp"
#include "resources/generated/anim/prop/target_balloon/anim_target_balloon.h"

namespace app {
namespace prop {

TargetBalloon::TargetBalloon(
    const FrameTimerT& frame_timer
) : anim_(frame_timer),
    sw_(frame_timer) {

    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_offset({0, 0});
    hitboxes[0].set_size({16, 16});
    hitboxes[0].set_enabled(false);
    gain_ = { 0.001, 0.001 };

    state_ = State::Inactive;
}

void TargetBalloon::spawn(const mgc::math::Vec2i& pos) {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(true);
    this->set_position(pos);

    anim_.set_anim_frames(anim_target_balloon_target_floating);
    anim_.start_animation();
    anim_.set_loop(true);
    anim_.set_current_frame(this->mut_sprite());
    this->mut_sprite().set_visible(true);

    velocity_ = { 0, 0 };

    MGC_ASSERT(
        countof(color_palette_) >= tileset_get_palette_count(this->sprite().tileset()),
        ""
    );
    tileset_copy_palette(
        this->sprite().tileset(),
        color_palette_,
        countof(color_palette_)
    );

    color_palette_[2] = MGC_COLOR_SWAP(color_);

    state_ = State::Active;
}

void TargetBalloon::despawn() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);

    state_ = State::Despawned;
}

void TargetBalloon::update_movement() {
    
    if ( state_ != State::Active ) return;
        
    auto pos = this->precise_position();

    auto dx = goal_.x - pos.x;
    auto dy = goal_.y - pos.y;

    auto r = sqrt(dx*dx + dy*dy);

    if ( r != 0 ) {
        dx /= r;
        dy /= r;
    }

    velocity_.x += gain_.x*dx;
    velocity_.y += gain_.y*dy;

    pos.x += velocity_.x;
    pos.y += velocity_.y;

    this->set_precise_position(pos);
}

void TargetBalloon::update_animation() {
    
    if ( state_ == State::Despawning ) {
        if ( anim_.is_finished() ) {
            state_ = State::Despawning2;
            this->mut_sprite().set_visible(false);
            sw_.restart();
        } else {
            anim_.proc();
            anim_.set_current_frame(this->mut_sprite());
        }
    } else if ( state_ == State::Despawning2 ) {
        if ( sw_.elapsed_ms() >= 5000 ) {
            despawn();
        }
    } else {
        anim_.proc();
        anim_.set_current_frame(this->mut_sprite());
    }
}

void TargetBalloon::on_attack_hit(
    const attack::Attack& attack,
    const mgc::collision::BoxCollisionInfo& info
) {
    if ( attack.can_break_block() ) {
        if ( state_ == State::Active ) {
            auto pos = this->position();
            pos += { -4, -4 };
            this->set_position(pos);

            auto& hitboxes = this->mut_hitboxes();
            hitboxes[0].set_enabled(false);

            anim_.set_anim_frames(anim_target_balloon_target_breaking);
            anim_.set_loop(false);
            anim_.set_current_frame(this->mut_sprite());
            anim_.start_animation();

            state_ = State::Despawning;
        }
    }
}

bool TargetBalloon::draw(
    mgc::graphics::Framebuffer &fb,
    const mgc::math::Vec2i &cam_pos,
    const mgc::graphics::DrawOptions *options
) const {

    (void)options;
    
    mgc::graphics::DrawOptions opt = {0};
    opt.eff_flags = static_cast<uint16_t>(DRAW_EFFECT_PALETTE_CHANGE);
    opt.palette_array = color_palette_;
    opt.palette_count = countof(color_palette_);

    return Prop::draw(fb, cam_pos, &opt);
}


} // namespace app
} // namespace block

