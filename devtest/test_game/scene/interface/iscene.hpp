#ifndef MGC_ISCENE_HPP
#define MGC_ISCENE_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_id.hpp"

namespace app {

struct IScene {
    virtual ~IScene() = default;
    virtual SceneId id() const = 0;
    virtual SceneId id_next() const = 0;
    virtual void set_id_next(SceneId id) = 0;
    virtual bool has_scene_change_request() const = 0;
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void draw(mgc::graphics::Framebuffer& fb) = 0;
};

} // namespace app

#endif/*MGC_ISCENE_HPP*/

