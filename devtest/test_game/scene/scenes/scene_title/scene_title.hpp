#ifndef MGC_SCENE_TITLE_HPP
#define MGC_SCENE_TITLE_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_base.hpp"
#include "game_context.hpp"

namespace app {

struct Scene_Title : SceneBase {

    explicit Scene_Title(GameContext& ctx);
    void init() override;
    void update() override;
    void draw(mgc::graphics::Framebuffer& fb) override;

private:
    enum class SelectedState {
        None,
        Start,
        Password
    } selected_state_ = SelectedState::None;
    mgc::parts::BasicLabel label_title_;
    mgc::parts::BasicLabel label_title_en_;
    mgc::parts::BasicSelectbox selectbox_menu_;
    static constexpr size_t SELECT_INDEX_START = 0;
    static constexpr size_t SELECT_INDEX_PASSWORD = 1;

    void update_select();
};

} // namespace app

#endif/*MGC_SCENE_TITLE_HPP*/

