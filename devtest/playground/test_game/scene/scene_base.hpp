#ifndef MGC_SCENE_BASE_HPP
#define MGC_SCENE_BASE_HPP

#include "mgc_cpp/mgc.hpp"
#include "game_context/game_context.hpp"
#include "scene_id.hpp"

namespace app {

struct SceneBase {
    virtual ~SceneBase() = default;

    virtual SceneId id() const = 0;
    virtual SceneId id_next() const = 0;
    virtual bool has_scene_change_request() const = 0;
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void draw(mgc::graphics::Framebuffer& fb) = 0;
};

} // namespace app

#endif/*MGC_SCENE_BASE_HPP*/

