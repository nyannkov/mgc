#include "app_common.hpp"
#include "talkflow_effects.hpp"

namespace app {

void TalkflowEffects::play_typing_sound(mgc_node_idx_t tag, bool is_scrolling) {
    if ( !is_scrolling ) {
        sound_controller_.play_sound_effect(typing_sound_id_);
    }
}

void TalkflowEffects::play_select_move_sound(mgc_node_idx_t tag) {
    sound_controller_.play_sound_effect(select_sound_id_);
}

void TalkflowEffects::play_choice_confirm_sound(mgc_node_idx_t tag) {
    sound_controller_.play_sound_effect(confirm_sound_id_);
}

} // namespace app

