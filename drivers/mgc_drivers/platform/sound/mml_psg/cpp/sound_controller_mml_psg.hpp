/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_DRIVERS_MML_PSG_CONTROLLER_HPP
#define MGC_DRIVERS_MML_PSG_CONTROLLER_HPP

#include "mgc_cpp/platform/sound/isound_controller.hpp"
#include "mgc_drivers/platform/sound/mml_psg/mml_psg.h"

namespace mgc {
namespace drivers {
namespace platform {
namespace sound {

struct IMmlPsgSoundControllerListener {
    virtual ~IMmlPsgSoundControllerListener() = default;

    virtual void on_background_music_user_command(uint8_t ch, int32_t param) {}
    virtual void on_sound_effect_user_command(uint8_t ch, int32_t param) {}

};

struct MmlPsgSoundController : mgc::platform::sound::ISoundController {

    MmlPsgSoundController() {}
    ~MmlPsgSoundController() = default;

    void on_background_music_callback(uint8_t ch, int32_t param) {
        if (listener_) {
            listener_->on_background_music_user_command(ch, param);
        }
    }

    void on_sound_effect_callback(uint8_t ch, int32_t param) {
        if (listener_) {
            listener_->on_sound_effect_user_command(ch, param);
        }
    }

    void init(float mml_proc_rate = 0.0F) {
        mml_psg_init(mml_proc_rate, this);
        mml_psg_set_callback_background_music([](uint8_t ch, int32_t param, void *ctx) {
            static_cast<MmlPsgSoundController*>(ctx)->on_background_music_callback(ch, param);
        });

        mml_psg_set_callback_sound_effect([](uint8_t ch, int32_t param, void *ctx) {
            static_cast<MmlPsgSoundController*>(ctx)->on_sound_effect_callback(ch, param);
        });
    }

    void deinit() {
        mml_psg_deinit();
    }

    void set_lpf_enabled(bool enabled) {
        mml_psg_set_psg_lpf_enabled(enabled);
    }

    void set_lpf_alpha(float alpha) {
        mml_psg_set_psg_lpf_alpha(alpha);
    }

    void set_background_music_list(const mgc_mml_record_t *records, size_t count) {
        mml_psg_set_background_music_list(records, count);
    }

    void set_sound_effect_list(const  mgc_mml_record_t *records, size_t count) {
        mml_psg_set_sound_effect_list(records, count);
    }

    void set_speed_factor(float factor) {
        
        mml_psg_set_speed_factor(static_cast<uint16_t>(factor)*100);
    }

    void shift_pitch_by_degree(int16_t degree) {
        mml_psg_shift_pitch_by_degree(degree);
    }

    void bind_listener(IMmlPsgSoundControllerListener& listener) {
        listener_ = &listener;
    }

    void unbind_listener() {
        listener_ = nullptr;
    }

    bool play_background_music(int music_id, float fade_in_sec) override {
        return mml_psg_play_background_music(music_id, fade_in_sec);
    }

    bool play_sound_effect(int effect_id, float fade_in_sec) override {
        return mml_psg_play_sound_effect(effect_id, fade_in_sec);
    }

    void stop_background_music(float fade_out_in_sec) override {
        mml_psg_stop_background_music(fade_out_in_sec);
    }

    void stop_all_sound_effects(float fade_out_in_sec) override {
        mml_psg_stop_all_sound_effects(fade_out_in_sec);
    }

    bool is_background_music_playing() const override {
        return mml_psg_is_background_music_playing();
    }

    bool is_any_sound_effect_playing() const override {
        return mml_psg_is_any_sound_effect_playing();
    }

    bool has_background_music_finished() const override {
        return mml_psg_has_background_music_finished();
    }

    bool have_all_sound_effects_finished() const override {
        return mml_psg_have_all_sound_effects_finished();
    }

    void set_master_volume(float volume) override {
        mml_psg_set_master_volume(volume);
    }

    void set_background_music_volume(float volume) override {
        mml_psg_set_background_music_volume(volume);
    }

    void set_sound_effect_volume(float volume) override {
        mml_psg_set_sound_effect_volume(volume);
    }

    float get_master_volume() const override {
        return mml_psg_get_master_volume();
    }

    float get_background_music_volume() const override {
        return mml_psg_get_background_music_volume();
    }

    float get_sound_effect_volume() const override {
        return mml_psg_get_sound_effect_volume();
    }

private:
    IMmlPsgSoundControllerListener *listener_;
};

}// namespace sound
}// namespace platform
}// namespace drivers
}// namespace mgc

#endif/*MGC_DRIVERS_MML_PSG_CONTROLLER_HPP*/
