#ifndef MGC_SCENE_TRANSITION_REQUEST_HPP
#define MGC_SCENE_TRANSITION_REQUEST_HPP

#include "app_common.hpp"
#include "scene/scene_id.hpp"

namespace app {

struct SceneTransitionRequest {
    SceneId id_next = SceneId::Title;
    uint32_t gate_id = 0;
};

} // namespace app

#endif// MGC_SCENE_TRANSITION_REQUEST_HPP

