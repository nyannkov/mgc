#ifndef MGC_SCENE_TRANSITION_REQUEST_HPP
#define MGC_SCENE_TRANSITION_REQUEST_HPP

#include "app_common.hpp"
#include "scene/scene_id.hpp"

namespace app {

struct SceneTransitionRequest {
    SceneId id_next;
};

} // namespace app

#endif// MGC_SCENE_TRANSITION_REQUEST_HPP

