#ifndef MGC_STAGE_INFO_HPP
#define MGC_STAGE_INFO_HPP

#include "scene/scene_id.hpp"

namespace app {

struct StageInfo {

    int32_t previous_floor() const { return previous_floor_; }
    int32_t current_floor() const { return current_floor_; }
    void set_current_floor(int32_t floor) { 
        previous_floor_ = current_floor_;
        current_floor_ = floor;
    }

    SceneId current_scene_id() const { return current_scene_id_; }
    void set_current_scene_id(SceneId id) {
        previous_scene_id_ = current_scene_id_;
        current_scene_id_ = id;
    }

    SceneId previous_scene_id() const { return previous_scene_id_; }
    SceneId next_scene_id() const { return next_scene_id_; }

    void set_next_scene_request(SceneId next_id) { 
        next_scene_request_ = true;
        next_scene_id_ = next_id;
    }
    void clear_next_scene_request() { next_scene_request_ = false; }
    bool next_scene_request() const { return next_scene_request_; }

private:
    int32_t current_floor_ = 0;
    int32_t previous_floor_ = 0;
    bool next_scene_request_ = false;
    SceneId current_scene_id_ = SceneId::Id_000;
    SceneId previous_scene_id_ = SceneId::Id_000;
    SceneId next_scene_id_ = SceneId::Id_000;
};

}// namespace app

#endif/*MGC_STAGE_INFO_HPP*/

