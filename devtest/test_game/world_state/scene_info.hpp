#ifndef MGC_SCENE_INFO_HPP
#define MGC_SCENE_INFO_HPP

#include "scene/scene_id.hpp"

namespace app {

struct SceneInfo {
    // Scene
    void set_prev_scene_id(SceneId id) { prev_scene_id_ = id; }
    SceneId prev_scene_id() const { return prev_scene_id_; }
    
private:
    SceneId prev_scene_id_ = SceneId::Title;
};

}// namespace app

#endif/*MGC_SCENE_INFO_HPP*/

