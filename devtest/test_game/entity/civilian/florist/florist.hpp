#ifndef MGC_CIVILIAN_FLORIST_HPP
#define MGC_CIVILIAN_FLORIST_HPP

#include <cstdio>
#include "app_common.hpp"
#include "world_state/world_state.hpp"
#include "entity/civilian/civilian.hpp"
#include "resources/generated/talkscript/talkscript_1.h"
#include "entity/player/player.hpp"
#include "scene/scene_id.hpp"

namespace app {
namespace civilian {

struct TalkflowListenerAtShopping : ITalkflowListenerT {
    explicit TalkflowListenerAtShopping(Player& player, WorldState& world_state)
        : player_(player),
          scene_info_(world_state.scene_info) {
    }

    const char *on_get_message_format(mgc_node_idx_t tag, const char *format_string) override {
        if ( tag == MGC_TALKSCRIPT_1_SHOP_SHOPPING_CHECK ) {
            snprintf(text_buffer, sizeof(text_buffer), format_string, last_item_value_);
        }
        return text_buffer;
    }
//    bool on_start_message(mgc_node_idx_t tag, DialogueboxConfig& dialoguebox_config) override;
//    bool on_start_choice(mgc_node_idx_t tag, SelectboxConfig& selectbox_config) override;
//    void on_message_done(mgc_node_idx_t tag) override;
    void on_choice_done(mgc_node_idx_t tag, size_t item_tag, int32_t value) override {
        switch ( tag ) {
        case MGC_TALKSCRIPT_1_TAG_CHOICE__SHOPPING_CONTENT_CASE_1:/*FALLTHROUGH*/
        case MGC_TALKSCRIPT_1_TAG_CHOICE__SHOPPING_CONTENT_CASE_2:/*FALLTHROUGH*/
            last_item_value_ = value;
            last_item_tag_ = item_tag;
            last_choice_tag_ = tag;
            break;
        }
    }
    int32_t on_select(mgc_node_idx_t tag) override { 
        if ( tag == MGC_TALKSCRIPT_1_TAG_SELECT__SELECT_SHOPPING_CONTENT ) {
            switch ( scene_info_.prev_scene_id() ) {
            case SceneId::Stage1_4:
                return MGC_TALKSCRIPT_1_TAG_SELECT__SELECT_SHOPPING_CONTENT__CASE__CASE_1;

            case SceneId::Stage2_4:
                return MGC_TALKSCRIPT_1_TAG_SELECT__SELECT_SHOPPING_CONTENT__CASE__CASE_2;

            default:
                return MGC_TALKSCRIPT_1_TAG_SELECT__SELECT_SHOPPING_CONTENT__CASE__CASE_1;
            }
        }
        return 0; 
    }
    bool on_decision(mgc_node_idx_t tag) override {
        if ( tag == MGC_TALKSCRIPT_1_TAG_DECISION__CHECK_OWNED ) {
            
            if ( last_choice_tag_ == MGC_TALKSCRIPT_1_TAG_CHOICE__SHOPPING_CONTENT_CASE_1 ) {
                switch ( last_item_tag_ ) {
                case MGC_TALKSCRIPT_1_TAG_CHOICE__SHOPPING_CONTENT_CASE_1__ITEM__COOKIE:
                    return player_.equipment_info().item.has_item_at(ItemId::Cookie);

                case MGC_TALKSCRIPT_1_TAG_CHOICE__SHOPPING_CONTENT_CASE_1__ITEM__BOOMERANG:
                    return player_.equipment_info().weapon.has_item_at(WeaponId::Boomerang);

                default:
                    return true;
                }

            } else if ( last_choice_tag_ == MGC_TALKSCRIPT_1_TAG_CHOICE__SHOPPING_CONTENT_CASE_2 ) {
                switch ( last_item_tag_ ) {
                case MGC_TALKSCRIPT_1_TAG_CHOICE__SHOPPING_CONTENT_CASE_2__ITEM__COOKIE:
                    return player_.equipment_info().item.has_item_at(ItemId::Cookie);

                case MGC_TALKSCRIPT_1_TAG_CHOICE__SHOPPING_CONTENT_CASE_2__ITEM__DIVING_EQUIPMENT:
                    return player_.equipment_info().item.has_item_at(ItemId::DivingEquipment);

                default:
                    return true;
                }

            } else { 
                return true;
            }

        } else if ( tag == MGC_TALKSCRIPT_1_TAG_DECISION__CHECK_MONEY ) {
            
            if ( player_.money() < last_item_value_ ) {
                return false;
            }
                
            player_.sub_money(last_item_value_);
            
            if ( last_choice_tag_ == MGC_TALKSCRIPT_1_TAG_CHOICE__SHOPPING_CONTENT_CASE_1 ) {
                switch ( last_item_tag_ ) {
                case MGC_TALKSCRIPT_1_TAG_CHOICE__SHOPPING_CONTENT_CASE_1__ITEM__COOKIE:
                    player_.equipment_info().item.add(ItemId::Cookie);
                    break;

                case MGC_TALKSCRIPT_1_TAG_CHOICE__SHOPPING_CONTENT_CASE_1__ITEM__BOOMERANG:
                    player_.equipment_info().weapon.add(WeaponId::Boomerang);
                    break;

                default:
                    break;
                }

            } else if ( last_choice_tag_ == MGC_TALKSCRIPT_1_TAG_CHOICE__SHOPPING_CONTENT_CASE_2 ) {
                switch ( last_item_tag_ ) {
                case MGC_TALKSCRIPT_1_TAG_CHOICE__SHOPPING_CONTENT_CASE_2__ITEM__COOKIE:
                    player_.equipment_info().item.add(ItemId::Cookie);
                    break;

                case MGC_TALKSCRIPT_1_TAG_CHOICE__SHOPPING_CONTENT_CASE_2__ITEM__DIVING_EQUIPMENT:
                    player_.equipment_info().item.add(ItemId::DivingEquipment);
                    break;

                default:
                    break;
                }
            } else { }

            return true;

        } else {
            return true;
        }
    }

    void on_flow_end(mgc_node_idx_t tag) override {
        if ( tag ==MGC_TALKSCRIPT_1_TAG_MESSAGE__START_COFFEE_BREAK ) {
            coffee_break_flag_ = true;
        }
    }

    bool coffee_break_flag() const { return coffee_break_flag_; }
    void clear_coffee_break_flag() { coffee_break_flag_ = false; }

private:
    Player& player_;
    const SceneInfo& scene_info_;
    int32_t last_item_value_ = 0;
    int32_t last_item_tag_ = 0;
    mgc_node_idx_t last_choice_tag_ = 0; 
    char text_buffer[128];
    bool coffee_break_flag_ = false;
};


// Florista
enum class FloristAnimState {
    Stand_Left,
    Stand_Right,
    LookUp_Right
};

struct Florist : Civilian {
    explicit Florist(
        const GamepadT& gamepad,
        WorldState& world_state,
        Player& player);
    ~Florist() = default;
    Florist(const Florist&) = delete;
    Florist& operator=(const Florist&) = delete;
    Florist(Florist&&) = default;
    Florist& operator=(Florist&&) = default;

    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn(const mgc::math::Vec2i& pos, FloristAnimState anim_state);
    void despawn() override;
    void update_movement() override;
    void update_animation() override;

    void set_anim_state(FloristAnimState anim_state);
    FloristAnimState anim_state() const { return anim_state_; }

    void on_player_in_view(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) override;

    bool coffee_break_flag() const { return talkflow_listener_.coffee_break_flag(); }
    void clear_coffee_break_flag() { talkflow_listener_.clear_coffee_break_flag(); }

private:
    const GamepadT& gamepad_;
    TalkflowListenerAtShopping talkflow_listener_;
    CheckpointInfo& cp_info_;
    FloristAnimState anim_state_;
    bool start_coffee_break_ = false;
};

}// namespace civilian
}// namespace app

#endif/*MGC_CIVILIAN_FLORIST_HPP*/

