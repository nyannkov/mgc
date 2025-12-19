#ifndef MGC_SCENE_000_HPP
#define MGC_SCENE_000_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_base.hpp"
#include "game_context/game_context.hpp"

namespace app {

struct Scene000 : SceneBase {

    explicit Scene000(GameContext& ctx);
    void init() override;
    void update() override;
    void draw(mgc::graphics::Framebuffer& fb) override;

private:
    mgc::parts::BasicLabel label_title_;
    mgc::parts::BasicLabel label_title_en_;
    mgc::parts::BasicSelectbox selectbox_menu_;
    bool change_wait_ = false;
};

} // namespace app

#endif/*MGC_SCENE_000_HPP*/

