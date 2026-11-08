#ifndef MGC_SCENE_INFO_HPP
#define MGC_SCENE_INFO_HPP

#include "scene/scene_id.hpp"

namespace app {

struct SceneInfo {
    // Scene
    void set_prev_scene_id(SceneId id) { prev_scene_id_ = id; }
    SceneId prev_scene_id() const { return prev_scene_id_; }

    void set_from_gate_id(uint32_t id) { from_gate_id_ = id; }
    uint32_t from_gate_id() const { return from_gate_id_; }

private:
    SceneId prev_scene_id_ = SceneId::Title;
    uint32_t from_gate_id_ = 0;
};

}// namespace app

#endif/*MGC_SCENE_INFO_HPP*/

