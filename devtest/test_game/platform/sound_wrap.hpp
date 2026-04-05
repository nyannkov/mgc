#ifndef MGC_SOUND_WRAP_H
#define MGC_SOUND_WRAP_H

#include "sound_controller_mml_psg.hpp"


namespace app {

using ISoundListenerT = mgc::drivers::platform::sound::IMmlPsgSoundControllerListener;
struct SoundListenerT : ISoundListenerT {
    void clear() {
        last_bgm_param_ = 0;
        update_bgm_param_count_ = 0;
    }
    void on_background_music_user_command(uint8_t ch, int32_t param) override {
        last_bgm_param_ = param;
        update_bgm_param_count_++;
    }

    int32_t last_bgm_param() const {
        return last_bgm_param_; // TODO fix dirty read
    }

    uint32_t update_bgm_param_count() const {
        return update_bgm_param_count_; // TODO fix dirty read
    }

private:
    int32_t last_bgm_param_ = 0;
    uint32_t update_bgm_param_count_ = 0;
};

struct SoundControllerWrapper {
    
    void init() {
        sound_.init();
        sound_.set_lpf_enabled(true);
        sound_.set_lpf_alpha(0.5);
        sound_.set_master_volume(1.0);

        sound_listener_.clear();
        sound_.bind_listener(sound_listener_);
    }

    void deinit() {
        sound_.deinit();
    }

    void set_background_music_list(const mgc_mml_record_t *records, size_t count) {
        sound_.set_background_music_list(records, count);
    }

    void set_sound_effect_list(const  mgc_mml_record_t *records, size_t count) {
        sound_.set_sound_effect_list(records, count);
    }

    bool play_background_music(int music_id) {
        sound_listener_.clear();
        return sound_.play_background_music(music_id);
    }

    int current_background_music_id() const {
        return sound_.current_background_music_id();
    }

    bool play_sound_effect(int effect_id) {
        return sound_.play_sound_effect(effect_id);
    }

    int current_sound_effect_id() const {
        return sound_.current_sound_effect_id();
    }

    void stop_background_music() {
        sound_.stop_background_music();
    }

    void stop_all_sound_effects() {
        sound_.stop_all_sound_effects();
    }

    bool is_background_music_playing() const {
        return sound_.is_background_music_playing();
    }

    bool is_any_sound_effect_playing() const {
        return sound_.is_any_sound_effect_playing();
    }

    bool has_background_music_finished() const {
        return sound_.has_background_music_finished();
    }

    bool have_all_sound_effects_finished() const {
        return sound_.have_all_sound_effects_finished();
    }

    int32_t last_bgm_param() const {
        return sound_listener_.last_bgm_param();
    }

    uint32_t update_bgm_param_count() const {
        return sound_listener_.update_bgm_param_count();
    }
    
private:
    mgc::drivers::platform::sound::MmlPsgSoundController sound_;
    SoundListenerT sound_listener_;
};


}//namespace app

#endif//MGC_SOUND_WRAP_H

