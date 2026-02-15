#include "scene_core.hpp"
#include "entity/enemy/enemy.hpp"
#include "entity/player/player_hitbox_index.hpp"
#include "entity/block/block.hpp"

namespace app {
namespace scene {

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

const TalkflowRequest* event_update(
    IEventObjects& events
) {
    event::Event* current = nullptr;
    const TalkflowRequest* request = nullptr;

    for ( auto* evt : events.events() ) {
        if ( evt->event_state() == event::EventState::Playing ) {
            current = evt;
            break;
        }
    }

    if ( current ) {
        current->update();
        request = current->take_talkflow_request();
    } else {
        for( auto* evt : events.events() ) {
            if ( evt->event_state() == event::EventState::NotStarted ) {
                evt->update();
                request = evt->take_talkflow_request();
            }
            if ( evt->event_state() == event::EventState::Playing ) {
                break;
            }
        }
    }
    return request;
}

void update(
    SceneContext& scx, 
    TalkflowControllerT& talkflow,
    CameraT* camera
) {

    if ( scx.evts ) {
        auto* talkflow_req = scene::event_update(*scx.evts);
        if ( talkflow_req ) {
            if ( talkflow_req->listener ) {
                talkflow.bind_listener(*talkflow_req->listener);
            } else {
                talkflow.unbind_listener();
            }
            if ( talkflow_req->effect_type == TalkflowEffectType::Mute ) {
                talkflow.unbind_effects();
            } else {
                talkflow.bind_effects(scx.talkflow_effects);
            }
            talkflow.set_talkscript(*talkflow_req->talkscript);
            talkflow.begin(talkflow_req->start_label);
        }
    }

    talkflow.proc();
    if ( talkflow.has_finished() ) {
        talkflow.reset_state();
    }

    if ( !talkflow.in_progress() ) {

        scx.player.update_movement();

        if ( scx.stage ) {
            scx.stage->detect_hit(
                scx.player,
                static_cast<size_t>(PlayerHitboxIndex::Body)
            );
        }

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
                if ( block->player_push_direction() != app::block::Block::PlayerPushDirection::None ) {
                    if ( block->can_move_group(scx.objs->blocks()) ) {
                        block->resolve_movement();
                        break;
                    }
                }
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
                ColBox2BoxT::detect_pair(*enemy, scx.player, static_cast<size_t>(PlayerHitboxIndex::Body));
                ColBox2BoxT::detect_pair(*enemy, scx.player.attack());
                if ( ( enemy->enemy_state() == enemy::EnemyState::Active ) &&
                     ( enemy->hp() <= 0 )
                ) {
                    enemy->despawn();
                    scx.player.add_gold(enemy->gold());
                }
            }
            for ( auto* prop : scx.objs->props() ) {
                ColBox2BoxT::detect_pair(*prop, scx.player, static_cast<size_t>(PlayerHitboxIndex::Body));
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
        }

        if ( scx.evts ) {
            for ( auto* cs : scx.evts->events() ) {
                ColBox2BoxT::detect_pair(*cs, scx.player, static_cast<size_t>(PlayerHitboxIndex::Body));
            }
        }

        if ( scx.objs ) {
            const TalkflowRequest* talkflow_req = nullptr;
            for ( auto* civil : scx.objs->civils() ) {
                talkflow_req = civil->take_talkflow_request();
                if ( talkflow_req ) {
                    break;
                }
            }

            if ( !talkflow_req ) {
                for ( auto* prop : scx.objs->props() ) {
                    talkflow_req = prop->take_talkflow_request();
                    if ( talkflow_req ) {
                        break;
                    }
                }
            }

            if ( talkflow_req ) {
                if ( talkflow_req->listener ) {
                    talkflow.bind_listener(*talkflow_req->listener);
                } else {
                    talkflow.unbind_listener();
                }
                if ( talkflow_req->effect_type == TalkflowEffectType::Mute ) {
                    talkflow.unbind_effects();
                } else {
                    talkflow.bind_effects(scx.talkflow_effects);
                }
                talkflow.set_talkscript(*talkflow_req->talkscript);
                talkflow.begin(talkflow_req->start_label);
            }
        }
    }

    if ( !scx.player.is_game_over() ) {
        if ( scx.player.hp() <= 0 ) {
            scx.player.set_game_over();
        }
    }

    scx.player.update_animation(talkflow.in_progress());
    if ( scx.objs ) {
        for ( auto* enemy : scx.objs->enemies() ) {
            enemy->update_animation();
        }
        for ( auto* block : scx.objs->blocks() ) {
            block->update_animation();
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

    if ( scx.stage ) {
        scx.stage->draw(fb, pos);
    }
    if ( scx.objs ) {
        scx.objs->draw(fb, pos);
    }
    // TODO event

    scx.player.draw(fb, pos);
    scx.player.attack().draw(fb, pos);

    if ( scx.objs ) {
        scx.objs->draw_after(fb, pos);
    }

    talkflow.draw(fb);

    game_over_effect.proc(fb);
}

} // namespace scene
} // namespace app

