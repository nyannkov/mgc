#include "scene_core.hpp"
#include "entity/enemy/enemy.hpp"
#include "entity/player/player_hitbox_index.hpp"
#include "entity/block/block.hpp"
#include "resources/mml/mml.h"

namespace app {
namespace scene {

namespace {

void set_talkflow_effect(TalkflowControllerT& talkflow, TalkflowEffects& talkflow_effects, TalkflowEffectType effect_type) {
    switch ( effect_type ) {
    case TalkflowEffectType::Mute:
        talkflow.unbind_effects();
        break;
    case TalkflowEffectType::Type1:
        talkflow_effects.set_typing_sound_id(MML_SE_11_SELECT_2);
        talkflow_effects.set_select_sound_id(MML_SE_9_SELECT);
        talkflow_effects.set_confirm_sound_id(MML_SE_12_SILENT);
        talkflow.bind_effects(talkflow_effects);
        break;
    default:
        talkflow_effects.set_typing_sound_id(MML_SE_9_SELECT);
        talkflow_effects.set_select_sound_id(MML_SE_9_SELECT);
        talkflow_effects.set_confirm_sound_id(MML_SE_9_SELECT);
        talkflow.bind_effects(talkflow_effects);
        break;
    }
}

bool event_update(IEventObjects* events) {
    bool is_control_locked = false;
    if ( events ) {
        for ( auto* evt : events->events() ) {
            evt->update();
            is_control_locked = is_control_locked || evt->is_control_locked();
        }
    }

    return is_control_locked;
}

bool is_just_off_board(SceneContext& scx) {
    if ( scx.objs ) {
        for ( auto* carrier : scx.objs->carriers() ) {
            if ( carrier->off_board_state() == carrier::Carrier::OffBoardState::JustOffBoard ) {
                return true;
            }
        }
    }
    return false;
}

void update_movement(SceneContext& scx) {

    scx.player.update_movement(is_just_off_board(scx));

    if ( scx.objs ) {
        for ( auto* block : scx.objs->blocks() ) {
            block->update_movement();
        }
        for ( auto* block : scx.objs->blocks() ) {
            scx.stage->detect_hit(
                *block,
                static_cast<size_t>(block::BlockHitboxIndex::Body)
            );
        }
        for ( size_t i = 0; i < scx.objs->blocks().size; ++i ) {
            auto* a = scx.objs->blocks().data[i];
            for ( size_t j = i + 1; j < scx.objs->blocks().size; ++j ) {
                auto* b = scx.objs->blocks().data[j];
                ColBox2BoxT::detect_pair(
                    *a,
                    static_cast<size_t>(block::BlockHitboxIndex::Body),
                    *b,
                    static_cast<size_t>(block::BlockHitboxIndex::Body)
                );
            }
        } 
        for ( auto* block : scx.objs->blocks() ) {
            scx.stage->detect_hit(
                *block,
                static_cast<size_t>(block::BlockHitboxIndex::Body)
            );
        }

        for ( auto* block : scx.objs->blocks() ) {
            ColBox2BoxT::detect_pair(*block, scx.player, static_cast<size_t>(PlayerHitboxIndex::Hand));
        }

        for ( auto* block : scx.objs->blocks() ) {
            ColBox2BoxT::detect_pair(*block, scx.player, static_cast<size_t>(PlayerHitboxIndex::Body));
        }
        ColBox2BoxT::detect_primary_ptr(    
            scx.player,
            static_cast<size_t>(PlayerHitboxIndex::Body),
            scx.player.velocity(),
            mgc::collision::BoxBoxPtrArrayView<block::Block>{ scx.objs->blocks().begin(), scx.objs->blocks().size },
            static_cast<size_t>(block::BlockHitboxIndex::Body)
        );

        for ( auto* enemy : scx.objs->enemies() ) {
            enemy->update_movement();
            scx.stage->detect_hit(
                *enemy,
                static_cast<size_t>(enemy::EnemyHitboxIndex::Body)
            );
            scx.stage->detect_hit(
                *enemy,
                static_cast<size_t>(enemy::EnemyHitboxIndex::Head)
            );

            ColBox2BoxT::detect_pair(*enemy, scx.player, static_cast<size_t>(PlayerHitboxIndex::Body));

            ColBox2BoxT::detect_pair(*enemy, scx.player.attack());
            for ( auto* weapon: enemy->weapons() ) {
                ColBox2BoxT::detect_pair(scx.player, *weapon);
            }

            if ( ( enemy->enemy_state() == enemy::EnemyState::Active ) &&
                 ( enemy->hp() <= 0 )
            ) {
                enemy->despawn();
                scx.player.add_money(enemy->money());
            }
        }
        ColBox2BoxT::detect_pair(scx.player, scx.player.attack());

        for ( auto* prop : scx.objs->props() ) {
            ColBox2BoxT::detect_pair(*prop, scx.player, static_cast<size_t>(PlayerHitboxIndex::Body));
            ColBox2BoxT::detect_pair(*prop, scx.player.attack());
        }
        for ( auto* civil : scx.objs->civils() ) {
            ColBox2BoxT::detect_pair(*civil, scx.player, static_cast<size_t>(PlayerHitboxIndex::Body));
        }
        for ( auto* item : scx.objs->items() ) {
            ColBox2BoxT::detect_pair(*item, scx.player, static_cast<size_t>(PlayerHitboxIndex::Body));
            if ( item->collected() ) {
                item->despawn();
            }
        }
        for ( auto* carrier : scx.objs->carriers() ) {
            carrier->update_movement();
        }
        for ( auto* carrier : scx.objs->carriers() ) {
            ColBox2MapT::detect(scx.player, static_cast<size_t>(PlayerHitboxIndex::Body), *carrier);
        }
    }

    if ( scx.stage ) {
        scx.stage->detect_hit(
            scx.player,
            static_cast<size_t>(PlayerHitboxIndex::Body)
        );
        scx.stage->detect_hit(
            scx.player,
            static_cast<size_t>(PlayerHitboxIndex::Head)
        );
    }

    if ( scx.evts ) {
        for ( auto* cs : scx.evts->events() ) {
            ColBox2BoxT::detect_pair(*cs, scx.player, static_cast<size_t>(PlayerHitboxIndex::Body));
        }
    }

    scx.player.resolve_movement();
    if ( scx.objs ) {
        for ( auto* block : scx.objs->blocks() ) {
            if ( block->player_push_direction() != app::block::Block::PlayerPushDirection::None ) {
                if ( block->can_move_group(scx.objs->blocks()) ) {
                    block->resolve_movement();
                    break;
                }
            }
        }
    }
}

const TalkflowRequest* take_talkflow_request(SceneContext& scx) {

    const TalkflowRequest* talkflow_req = nullptr;
    if ( scx.evts ) {
        for ( auto* evt : scx.evts->events() ) {
            talkflow_req = evt->take_talkflow_request();
            if ( talkflow_req ) {
                return talkflow_req;
            }
        }
    }

    if ( scx.objs ) {
        for ( auto* civil : scx.objs->civils() ) {
            talkflow_req = civil->take_talkflow_request();
            if ( talkflow_req ) {
                return talkflow_req;
            }
        }

        for ( auto* prop : scx.objs->props() ) {
            talkflow_req = prop->take_talkflow_request();
            if ( talkflow_req ) {
                return talkflow_req;
            }
        }
    }

    return nullptr;
}

} // namespace


void setup_scene_context(
    SceneContext& scx, 
    stage::Stage* stage,
    ISceneObjects* objs,
    IEventObjects* evts
) {
    scx.stage = stage;
    scx.objs = objs;
    scx.evts = evts;
}

void update(
    SceneContext& scx, 
    TalkflowControllerT& talkflow,
    CameraT* camera
) {
    bool is_control_locked = false;

    is_control_locked = scene::event_update(scx.evts);
    if ( is_control_locked ) {
        scx.player.set_input_enabled(false);
    } else {
        scx.player.set_input_enabled(true);
    }

    if ( !talkflow.in_progress() ) {

        update_movement(scx);

        const auto* talkflow_req = take_talkflow_request(scx);

        if ( talkflow_req ) {
            if ( talkflow_req->listener ) {
                talkflow.bind_listener(*talkflow_req->listener);
            } else {
                talkflow.unbind_listener();
            }
            set_talkflow_effect(talkflow, scx.talkflow_effects, talkflow_req->effect_type);
            talkflow.set_talkscript(*talkflow_req->talkscript);
            talkflow.begin(talkflow_req->start_label);
        }
    }

    talkflow.proc();
    if ( talkflow.has_finished() ) {
        talkflow.reset_state();
    }

    if ( !scx.player.is_game_over() ) {
        if ( scx.player.hp() <= 0 ) {
            scx.player.set_game_over();
        }
    }

    scx.player.update_animation(talkflow.in_progress(), is_just_off_board(scx));

    if ( scx.objs ) {
        for ( auto* enemy : scx.objs->enemies() ) {
            enemy->update_animation();
        }
        for ( auto* block : scx.objs->blocks() ) {
            block->update_animation();
        }
        for ( auto* prop : scx.objs->props() ) {
            prop->update_animation();
        }
    }

    if ( camera ) {
        camera->update_follow_position();
    }
}

bool check_scene_transition_request(
    const SceneContext& scx, 
    const GameOverEffect& game_over_effect,
    SceneId& id_next
) {
    const SceneTransitionRequest * request = nullptr;
    if ( scx.objs ) {
        for ( auto* prop : scx.objs->props() ) {
            request = prop->take_scene_transition_request();
            if ( request ) {
                id_next = request->id_next;
                return true;
            }
        }
    }

    if ( scx.evts ) {
        for ( auto* cs : scx.evts->events() ) {
            request = cs->take_scene_transition_request();
            if ( request ) {
                id_next = request->id_next;
                return true;
            }
        }
    }

    if ( game_over_effect.has_finished() ) {
        id_next = SceneId::GameOver;
        return true;
    }

    return false;
}

void draw(
    ColorT back_color,
    FramebufferT& fb,
    const CameraT* camera,
    const SceneContext& scx, 
    const TalkflowControllerT& talkflow,
    GameOverEffect& game_over_effect
) {

    mgc::math::Vec2i pos = {0, 0};

    fb.clear(back_color);

    if ( camera ) {
        pos = camera->follow_position();
    }

    if ( scx.objs ) {
        scx.objs->draw_before(fb, pos);
    }

    if ( scx.stage ) {
        scx.stage->draw(fb, pos);
    }
    if ( scx.objs ) {
        scx.objs->draw(fb, pos);
    }
    // TODO event

    scx.player.draw(fb, pos);
    scx.player.attack().draw_wrap(fb, pos);

    if ( scx.objs ) {
        scx.objs->draw_after(fb, pos);
    }

    if ( scx.stage ) {
        scx.stage->draw_after(fb, pos);
    }

    if ( scx.evts ) {
        scx.evts->draw_effect(fb, pos);
    }

    talkflow.draw(fb);

    game_over_effect.proc(fb);
}

} // namespace scene
} // namespace app

