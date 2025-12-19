#ifndef MGC_DEFAULT_TALKFLOW_EFFECTS_HPP
#define MGC_DEFAULT_TALKFLOW_EFFECTS_HPP

#include "app_common.hpp"

namespace app {

struct DefaultTalkflowEffects : ITalkflowEffectsT {
    explicit DefaultTalkflowEffects(SoundControllerT& sound_controller) 
        : sound_controller_(sound_controller) {
    }
    void play_typing_sound(mgc_node_idx_t tag, bool is_scrolling) override;
    void play_select_move_sound(mgc_node_idx_t tag) override;
    void play_choice_confirm_sound(mgc_node_idx_t tag) override;
    
private:
    SoundControllerT& sound_controller_;
};

} // namespace app

#endif// MGC_DEFAULT_TALKFLOW_EFFECTS_HPP

