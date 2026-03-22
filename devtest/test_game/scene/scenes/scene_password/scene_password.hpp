#ifndef MGC_SCENE_PASSWORD_HPP
#define MGC_SCENE_PASSWORD_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_base.hpp"
#include "record/save_data.hpp"
#include "game_context.hpp"

namespace app {

struct Scene_Password : SceneBase {

    explicit Scene_Password(GameContext& ctx);
    void init() override;
    void update() override;
    void draw(FramebufferT& fb) override;

private:
    struct TalkflowListener : ITalkflowListenerT {
        void on_choice_done(
            mgc_node_idx_t tag, 
            size_t item_tag,
            int32_t value
        ) override {
            choice_item_tag_ = item_tag;
            is_choice_done_ = true;
        }
        bool is_choice_done() const { return is_choice_done_; }
        size_t choice_item_tag() const { return choice_item_tag_; }
        void clear() {
            is_choice_done_ = false;
            choice_item_tag_ = 0;
        }
    private:
        bool is_choice_done_ = false;
        size_t choice_item_tag_ = 0;
    } talkflow_listener_;

    enum class InputState {
        None,
        ConfirmExit
    } input_state_ = InputState::None;

    WorldState& world_state_;
    LabelT label_button_;
    DialogueboxT diag_input_;
    char input_buffer_[32];
    EncodedSaveData enc_data_{};
    size_t cursor_input_text_ = 0;
    int selected_index_ = 0;
    static constexpr const char * const BUTTON_TEXT[36+2] = {
        "1 ", "2 ", "3 ", "4 ", "5 ", "6 ", "7 ", "8 ", "9 ", "0 ",
        "A ", "B ", "C ", "D ", "E ", "F ", "G ", "H ", "I ", "J ",
        "K ", "L ", "M ", "N ", "O ", "P ", "Q ", "R ", "S ", "T ",
        "U ", "V ", "W ", "X ", "Y ", "Z ",
        "OK ", "BACK "
    };
    static constexpr size_t BUTTON_INDEX_OK = 36;
    static constexpr size_t BUTTON_INDEX_BACK = 37;

    void update_input();
    void clear_input_text();
    void add_input_text(char c);
    void backspace_input_text();
    bool is_input_text_full() const;
    char* get_cursor_buffer(size_t cursor);
    void draw_button(FramebufferT& fb, LabelT& label, size_t button_index);

};

} // namespace app

#endif/*MGC_SCENE_PASSWORD_HPP*/

