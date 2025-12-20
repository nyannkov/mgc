#include "app_common.hpp"
#include "default_talkflow_effects.hpp"
#include "resources/mml/mml.h"

namespace app {

void DefaultTalkflowEffects::play_typing_sound(mgc_node_idx_t tag, bool is_scrolling) {
    if ( !is_scrolling ) {
        sound_controller_.play_sound_effect(MML_SE_9_SELECT, 0.0);
    }
}

void DefaultTalkflowEffects::play_select_move_sound(mgc_node_idx_t tag) {
    sound_controller_.play_sound_effect(MML_SE_9_SELECT, 0.0);
}

void DefaultTalkflowEffects::play_choice_confirm_sound(mgc_node_idx_t tag) {
    sound_controller_.play_sound_effect(MML_SE_9_SELECT, 0.0);
}

} // namespace app

