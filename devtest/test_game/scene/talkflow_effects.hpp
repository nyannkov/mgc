#ifndef MGC_TALKFLOW_EFFECTS_HPP
#define MGC_TALKFLOW_EFFECTS_HPP

#include "app_common.hpp"

namespace app {

struct TalkflowEffects : ITalkflowEffectsT {
    explicit TalkflowEffects(SoundControllerT& sound_controller) 
        : sound_controller_(sound_controller) {
    }
    void play_typing_sound(mgc_node_idx_t tag, bool is_scrolling) override;
    void play_select_move_sound(mgc_node_idx_t tag) override;
    void play_choice_confirm_sound(mgc_node_idx_t tag) override;

    void set_typing_sound_id(int id) { typing_sound_id_ = id; }
    int typing_sound_id() const { return typing_sound_id_; }

    void set_select_sound_id(int id) { select_sound_id_ = id; }
    int select_sound_id() const { return select_sound_id_; }

    void set_confirm_sound_id(int id) { confirm_sound_id_ = id; }
    int confirm_sound_id() const { return confirm_sound_id_; }
    
private:
    SoundControllerT& sound_controller_;
    int typing_sound_id_;
    int select_sound_id_;
    int confirm_sound_id_;
};

} // namespace app

#endif// MGC_TALKFLOW_EFFECTS_HPP

