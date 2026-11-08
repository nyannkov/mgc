#ifndef MGC_ISCENE_HPP
#define MGC_ISCENE_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_id.hpp"
#include "scene/request/scene_transition_request.hpp"

namespace app {

struct IScene {
    virtual ~IScene() = default;
    virtual SceneId id() const = 0;
    virtual bool has_scene_trans_request() const = 0;
    virtual SceneTransitionRequest scene_trans_req_info() const = 0;

    virtual void set_menu_request() = 0;
    virtual void clear_menu_request() = 0;
    virtual bool has_menu_request() const = 0;
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void draw(mgc::graphics::Framebuffer& fb) = 0;
};

} // namespace app

#endif/*MGC_ISCENE_HPP*/

