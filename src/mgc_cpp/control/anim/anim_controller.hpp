/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_CONTROL_ANIM_ANIM_CONTROLLER_HPP
#define MGC_CONTROL_ANIM_ANIM_CONTROLLER_HPP

#include "mgc/sequencer/animctrl.h"
#include "mgc_cpp/parts/assets/anim_frames.hpp"
#include "mgc_cpp/features/resettable.hpp"

namespace mgc {
namespace control {
namespace anim {

template <typename FrameTimerT>
struct AnimController : mgc::features::Resettable {

    AnimController(const FrameTimerT& timer) : timer_(timer) { reset(); }
    ~AnimController() = default;
    AnimController(const AnimController&) = delete;
    AnimController& operator=(const AnimController&) = delete;
    AnimController(AnimController&&) = default;
    AnimController& operator=(AnimController&&) = default;

    void set_anim_frames(const mgc::parts::assets::AnimFrames& frames) {
        animctrl_set_anim_frames(&animctrl_, &frames);
    }

    void set_speed(float speed) {
        animctrl_set_speed(&animctrl_, speed);
    }

    void set_loop(bool loop) {
        animctrl_set_loop(&animctrl_, loop);
    }

    void reset_animation() {
        animctrl_reset(&animctrl_);
    }

    void start_animation() {
        animctrl_start(&animctrl_, timer_.now_ms32());
    }

    void pause_animation() {
        animctrl_pause(&animctrl_);
    }

    void resume_animation() {
        animctrl_resume(&animctrl_, timer_.now_ms32());
    }

    bool proc() {
        return animctrl_proc(&animctrl_, timer_.now_ms32());
    }

    void seek_to_time_ms(uint32_t time_ms) {
        animctrl_seek_to_time_ms(&animctrl_, time_ms);
    }

    void seek_to_frame(size_t frame_index) {
        animctrl_seek_to_frame(&animctrl_, frame_index);
    }

    const mgc::parts::assets::Tileset* tileset() const {
        return animctrl_get_tileset(&animctrl_);
    }

    size_t current_tile_index() const {
        return animctrl_get_current_tile_index(&animctrl_);
    }

    template <typename SpriteT>
    void set_current_frame(SpriteT& sprite) {
        static_assert(std::is_base_of<mgc::parts::interfaces::mixins::WithTileset<SpriteT>, SpriteT>::value,
                      "SpriteT must inherit from WithTileset<SpriteT>");

        static_assert(std::is_base_of<mgc::parts::interfaces::mixins::WithTileIndexing<SpriteT>, SpriteT>::value,
                      "SpriteT must inherit from WithTileIndexing<SpriteT>");

        const auto* tileset = this->tileset();
        if ( tileset ) {
            sprite.set_tileset(*tileset);
            sprite.set_tile_index(this->current_tile_index());
        }
    }

    bool is_finished() const {
        return animctrl_is_finished(&animctrl_);
    }

    bool is_paused() const {
        return animctrl_is_paused(&animctrl_);
    }

    // [feature] Resettable
    void reset() override {
        animctrl_init(&animctrl_);
    }

private:
    mgc_animctrl_t animctrl_;
    const FrameTimerT& timer_;
};


}// namespace anim
}// namespace control
}// namespace mgc

#endif/*MGC_CONTROL_ANIM_ANIM_CONTROLLER_HPP*/
