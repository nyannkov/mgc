#ifndef MGC_ITEM_SLOT_HPP
#define MGC_ITEM_SLOT_HPP

#include "game_context.hpp"
#include "resources/menu/item_descriptors.h"
#include "resources/generated/tileset/tileset_menu_cursor.h"
#include "resources/generated/font/k8x12.h"

namespace app {

enum class SelectionState {
    Selecting,
    Confirmed,
    Cancelled,
    Exit
};

using mgc::platform::input::Key;

template <typename SlotT, size_t Row, size_t Col>
struct ItemSlot {

    explicit ItemSlot(SlotT& slot, const item_descriptors_t& desc)
        : slot_(slot),
          desc_(desc) { }
    
    void init(bool focused, mgc::math::Vec2i pos) {
        
        position_ = pos;

        equipped_item_cursor_.set_tileset(tileset_menu_cursor);
        equipped_item_cursor_.set_tile_index(0);
        if ( slot_.has_item_at(slot_.equipped_id()) ) {
            set_cursor_position(equipped_item_cursor_, slot_.equipped_id());
            equipped_item_cursor_.set_visible(true);
        } else {
            equipped_item_cursor_.set_visible(false);
        }

        selecting_item_cursor_.set_tileset(tileset_menu_cursor);
        selecting_item_cursor_.set_tile_index(1);
        selecting_item_cursor_.set_visible(false);

        title_.set_text(desc_.title);
        title_.adjust_size_to_fit();
        title_.set_position(position_);
        title_.set_font(k8x12);
        title_.set_fontsize2x(false);

        item_.set_tileset(*desc_.tileset);

        focused ? focus() : unfocus();

        selection_state_ = SelectionState::Cancelled;
    }

    void update(const GamepadT& gamepad) {
        if ( !focused_ ) return;
        if ( slot_.is_empty() ) return;


        if ( ( selection_state_ == SelectionState::Confirmed ) ||
             ( selection_state_ == SelectionState::Cancelled )
        ) {
            if (  gamepad.just_pressed(Key::Right) || 
                  gamepad.just_pressed(Key::Left) || 
                  gamepad.just_pressed(Key::Up) || 
                  gamepad.just_pressed(Key::Down) 
            ) {
                set_selection_state(SelectionState::Selecting);
            } else if ( gamepad.just_pressed(Key::Cancel) ) {
                set_selection_state(SelectionState::Exit);
            } else { }

        } else if ( selection_state_ == SelectionState::Selecting ) {
            if ( gamepad.just_pressed(Key::Enter) ) {
                if ( slot_.has_item_at(selecting_item_cursor_id_) ) {
                    set_selection_state(SelectionState::Confirmed);
                }
            } else if ( gamepad.just_pressed(Key::Cancel) ) {
                set_selection_state(SelectionState::Cancelled);
            } else { }
        } else { }

        switch (selection_state_) {
        case SelectionState::Selecting:
            select_item(gamepad);
            break;

        case SelectionState::Confirmed:
            confirm_item();
            break;

        case SelectionState::Cancelled:
            restore_cursor_position();
            break;

        default:
            break;
        }
    }

    void draw(FramebufferT& fb) {
        draw_items(fb);
        equipped_item_cursor_.draw(fb);
        selecting_item_cursor_.draw(fb);
        title_.draw(fb);
    }

    void focus() {
        focused_ = true;
        title_.set_font_fore_color(MGC_COLOR_WHITE);
        set_selection_state(SelectionState::Cancelled);
    }

    void unfocus() {
        focused_ = false;
        title_.set_font_fore_color(MGC_COLOR(0.5, 0.5, 0.5));
        set_selection_state(SelectionState::Cancelled);
    }

    bool is_focused() const { return focused_; }
    const item_descriptor_t* current_desc() const {
        if ( slot_.is_empty() ) {
            return nullptr;
        }

        uint32_t selected_id = 0;
        
        if ( selection_state_ == SelectionState::Selecting ) {
            selected_id = selecting_item_cursor_id_;
        } else {
            selected_id = slot_.equipped_id();
        }
        
        if ( slot_.has_item_at(selected_id) ) {
            return &desc_.descriptors[selected_id];
        } else {
            return nullptr;
        }
    }

    SelectionState selection_state() const { return selection_state_; }

private:
    SlotT& slot_;
    const item_descriptors_t& desc_;
    mgc::math::Vec2i position_;
    const size_t col_count = Col;
    const size_t row_count = Row;
    LabelT title_;
    SpriteT item_;
    SpriteT equipped_item_cursor_;
    SpriteT selecting_item_cursor_;
    uint32_t selecting_item_cursor_id_ = 0;
    bool focused_ = false;
    SelectionState selection_state_ = SelectionState::Cancelled;

    void draw_items(FramebufferT& fb) {
        uint32_t i = 0;
        mgc::math::Vec2i pos_org {};
        pos_org.x = position_.x + 5;
        pos_org.y = position_.y + 16;

        for ( size_t y = 0; y < row_count; y++ ) {
            for ( size_t x = 0; x < col_count; x++ ) {
                if ( slot_.has_item_at(i) ) {
                    mgc::math::Vec2i item_pos;
                    item_pos.x = 16 * x + pos_org.x;
                    item_pos.y = 16 * y + pos_org.y;
                    item_.set_tile_index(desc_.descriptors[i].tile_idx);
                    item_.set_position(item_pos);
                    item_.draw(fb);
                }
                i++;
                if ( i >= desc_.descriptor_count ) {
                    return;
                }
            }
        }
    }

    void set_selection_state(SelectionState state) {
        selecting_item_cursor_.set_visible(state == SelectionState::Selecting);
        selection_state_ = state;
    }

    void select_item(const GamepadT& gamepad) {
        if ( selection_state_ != SelectionState::Selecting ) {
            return;
        }

        uint32_t next_id = selecting_item_cursor_id_;
        uint32_t row_num = selecting_item_cursor_id_ / col_count;
        uint32_t col_num = selecting_item_cursor_id_ % col_count;

        if ( gamepad.just_pressed(Key::Right) ) {
            next_id++;
            if ( next_id >= desc_.descriptor_count ) {
                next_id = 0;
            }
        } else if ( gamepad.just_pressed(Key::Left) ) {
            if ( next_id == 0 ) {
                next_id = desc_.descriptor_count - 1;
            } else {
                next_id--;
            }
        } else if ( gamepad.just_pressed(Key::Up) ) {
            if ( row_num == 0 ) {
                row_num = row_count - 1;
            } else {
                row_num--;
            }
            next_id = row_num * col_count + col_num;
        } else if ( gamepad.just_pressed(Key::Down) ) {
            if ( row_num == row_count-1 ) {
                row_num = 0;
            } else {
                row_num++;
            }
            next_id = row_num * col_count + col_num;
        }

        selecting_item_cursor_id_ = next_id;
        set_cursor_position(selecting_item_cursor_, selecting_item_cursor_id_);
    }

    void confirm_item() {
        if ( slot_.equip(selecting_item_cursor_id_) ) {
            set_cursor_position(equipped_item_cursor_, selecting_item_cursor_id_);
        }
    }

    void restore_cursor_position() {
        selecting_item_cursor_id_ = slot_.equipped_id();
    }

    void set_cursor_position(SpriteT& cursor, uint32_t item_id) {
        mgc::math::Vec2i pos{};
        pos.x = (item_id%col_count) * 16 + position_.x + 5;
        pos.y = (item_id/col_count) * 16 + position_.y + 16;
        cursor.set_position(pos);
    }
};

}// namespace app

#endif/*MGC_ITEM_SLOT_HPP*/

