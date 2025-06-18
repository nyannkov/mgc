/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PLATFORM_SOUND_SOUND_CONTROLLER_HPP
#define MGC_PLATFORM_SOUND_SOUND_CONTROLLER_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace platform {
namespace sound {

struct ISoundController {

    virtual ~ISoundController() = default;
    virtual bool play_background_music(int music_id, float fade_in_sec) = 0;
    virtual bool play_sound_effect(int effect_id, float fade_in_sec) = 0;

    virtual void stop_background_music(float fade_out_in_sec) = 0;
    virtual void stop_all_sound_effects(float fade_out_in_sec) = 0;

    virtual bool is_background_music_playing() const = 0;
    virtual bool is_any_sound_effect_playing() const = 0;

    virtual bool has_background_music_finished() const = 0;
    virtual bool have_all_sound_effects_finished() const = 0;

    virtual void set_master_volume(float volume) = 0;
    virtual void set_background_music_volume(float volume) = 0;
    virtual void set_sound_effect_volume(float volume) = 0;

    virtual float master_volume() const = 0;
    virtual float background_music_volume() const = 0;
    virtual float sound_effect_volume() const = 0;

    virtual void proc() {}
};

}// namespace sound
}// namespace platform
}// namespace mgc

#endif/*MGC_PLATFORM_SOUND_SOUND_CONTROLLER_HPP*/
