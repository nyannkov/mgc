#ifndef MGC_TALKFLOW_REQUEST_HPP
#define MGC_TALKFLOW_REQUEST_HPP

#include "app_common.hpp"

namespace app {

enum class TalkflowEffectType {
    Default,
    Mute,
};

struct TalkflowRequest {
    const mgc::parts::assets::TalkScript* talkscript;
    mgc_node_idx_t start_label;
    ITalkflowListenerT* listener;
    TalkflowEffectType effect_type;
};

} // namespace app

#endif// MGC_TALKFLOW_REQUEST_HPP

