#ifndef MGC_SCENE_GAMEOVER_HPP
#define MGC_SCENE_GAMEOVER_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_base.hpp"
#include "game_context.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

struct Scene_GameOver : SceneBase {

    explicit Scene_GameOver(GameContext& ctx);
    void init() override;
    void update() override;
    void draw(mgc::graphics::Framebuffer& fb) override;

private:
    Player& player_;
    SceneId id_next_;
    struct TalkflowListener : ITalkflowListenerT {
        enum class GameRequest {
            None,
            Continue,
            Exit,
        };
        void on_choice_done(
            mgc_node_idx_t tag,
            size_t item_tag,
            int32_t value
        ) override {
            if ( tag == MGC_TALKSCRIPT_1_TAG_CHOICE__TRY_AGAIN ) {
                if ( item_tag == MGC_TALKSCRIPT_1_TAG_CHOICE__TRY_AGAIN__ITEM__YES ) {
                    request_ = GameRequest::Continue;
                } else {
                    request_ = GameRequest::Exit;
                }
            }
        }
        GameRequest game_request() const { return request_; }
        void clear_game_request() { request_ = GameRequest::None; }
    private:
        GameRequest request_ = GameRequest::None;
    } talkflow_listener_;
};

} // namespace app

#endif/*MGC_SCENE_GAMEOVER_HPP*/

