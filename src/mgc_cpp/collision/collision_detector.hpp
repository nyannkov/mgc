/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_COLLISION_COLLISION_DETECTOR_HPP
#define MGC_COLLISION_COLLISION_DETECTOR_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/physics/vec2.hpp"
#include "mgc_cpp/collision/hitbox.hpp"
#include "mgc_cpp/collision/collision_map.hpp"
#include "mgc_cpp/collision/map_collision_info.hpp"

namespace mgc {
namespace collision {

struct CollisionDetectorBoxToBox {
    template <typename T1, typename T2>
    static void detect(T1& obj1, T2& obj2) {
        static_assert(std::is_base_of<mgc::entities::mixins::WithHitboxes<T1, T1::HitboxCount>, T1>::value,
                      "T1 must inherit from WithHitboxes<T1, N>");

        static_assert(std::is_base_of<mgc::entities::mixins::WithOnHitBoxToBoxResponse<T1>, T1>::value,
                      "T1 must inherit from WithOnHitHitboxResponse<T1>");

        static_assert(std::is_base_of<mgc::entities::mixins::WithHitboxes<T2, T2::HitboxCount>, T2>::value,
                      "T2 must inherit from WithHitboxes<T2, N>");

        static_assert(std::is_base_of<mgc::entities::mixins::WithOnHitBoxToBoxResponse<T2>, T2>::value,
                      "T2 must inherit from WithOnHitHitboxResponse<T2>");

        const auto& obj1_hitboxes = obj1.hitboxes();
        const auto& obj2_hitboxes = obj2.hitboxes();

        for ( auto& h1 : obj1_hitboxes ) {
            if ( !hitbox_is_enabled(&h1) ) {
                continue;
            }
            for ( auto& h2 : obj2_hitboxes ) {
                if ( !hitbox_is_enabled(&h2) ) {
                    continue;
                }
                int32_t l1 = obj1.get_position().x + h1.x0_ofs;
                int32_t r1 = l1 + h1.width - 1;
                int32_t t1 = obj1.get_position().y + h1.y0_ofs;
                int32_t b1 = t1 + h1.height - 1;

                int32_t l2 = obj2.get_position().x + h2.x0_ofs;
                int32_t r2 = l2 + h2.width - 1;
                int32_t t2 = obj2.get_position().y + h2.y0_ofs;
                int32_t b2 = t2 + h2.height - 1;

                if ((l1 <= r2) && (l2 <= r1) && (t1 <= b2) && (t2 <= b1)) {
                    const mgc::collision::BoxCollisionInfo info1 = {h1, h2};
                    obj1.on_hit_box_to_box(obj2, info1);

                    const mgc::collision::BoxCollisionInfo info2 = {h2, h1};
                    obj2.on_hit_box_to_box(obj1, info2);
                }
            }
        }
    }
};

struct CollisionDetectorBoxToMap {

    enum class CornerPushDirection {
        PreferX,
        PreferY,
        None
    };

    CollisionDetectorBoxToMap() { this->init(); }
    ~CollisionDetectorBoxToMap() = default;

    template <typename ObjT, typename MapT>
    void detect(ObjT& obj, MapT& map, CornerPushDirection push_dir = CornerPushDirection::None) {
        if ( !map.get_collision_enabled() ) {
            return;
        }
        auto* collision_map = map.get_collision_map();
        if ( !collision_map ) {
            return;
        }
        const auto& obj_hitboxes = obj.hitboxes();

        this->init();

        for ( auto& h : obj_hitboxes ) {
            if ( !hitbox_is_enabled(&h) ) {
                continue;
            }

            ql_ = MGC_DIV_CELL_LEN((int32_t)obj.get_position().x + h.x0_ofs - map.get_position().x);
            qr_ = MGC_DIV_CELL_LEN((int32_t)obj.get_position().x + h.x0_ofs + h.width  - 1 - map.get_position().x);
            qt_ = MGC_DIV_CELL_LEN((int32_t)obj.get_position().y + h.y0_ofs - map.get_position().y);
            qb_ = MGC_DIV_CELL_LEN((int32_t)obj.get_position().y + h.y0_ofs + h.height - 1 - map.get_position().y);
            if ( ql_ < 0 ) ql_ = 0;
            if ( qt_ < 0 ) qt_ = 0;
            if ( collision_map->map_width <= qr_ ) qr_ = collision_map->map_width - 1;
            if ( collision_map->map_height <= qb_ ) qb_ = collision_map->map_height - 1;

            for ( uint16_t j = qt_ ; j <= qb_; j++ ) {
                for ( uint16_t i = ql_; i <= qr_; i++ ) {
                    uint8_t tile_id = map_decompress_and_get_tile_id(collision_map, i, j);
                    if ( (tile_id  & 0x80) != 0 ) {
                        this->update_hit_flags_and_counter(i, j);

                        const mgc::collision::MapCollisionInfo info = {h, tile_id, j, i};
                        obj.on_hit_box_to_map(obj, map, info);
                        map.on_hit_box_to_map(obj, map, info);
                    }
                }
            }

            mgc::physics::Vec2 pushback = calc_wall_pushback(obj, h, map, push_dir);
            const mgc::collision::MapPushbackInfo info = { h, pushback };
            obj.handle_map_pushback_result(obj, map, info);
            map.handle_map_pushback_result(obj, map, info);
        }
    }


private:
    bool is_hit_r_, is_hit_l_, is_hit_t_, is_hit_b_;
    bool is_hit_tr_, is_hit_tl_, is_hit_br_, is_hit_bl_;
    uint16_t hit_count_;
    int16_t qr_, ql_, qt_, qb_;

    void init() {
        is_hit_r_ = false;
        is_hit_l_ = false;
        is_hit_t_ = false;
        is_hit_b_ = false;
        is_hit_tr_ = false;
        is_hit_tl_ = false;
        is_hit_br_ = false;
        is_hit_bl_ = false;
        hit_count_ = 0;
        qr_ = 0;
        ql_ = 0;
        qt_ = 0;
        qb_ = 0;
    }

    void update_hit_flags_and_counter(uint16_t i, uint16_t j) {
        if ( i == ql_ ) {
            if ( j == qt_ ) {
                if ( !is_hit_tl_ ) {
                    is_hit_tl_ = true;
                    hit_count_++;
                }
            } else if ( j == qb_ ) {
                if ( !is_hit_bl_ ) {
                    is_hit_bl_ = true;
                    hit_count_++;
                }
            } else {
                if ( !is_hit_l_ ) {
                    is_hit_l_ = true;
                    hit_count_++;
                }
            }
        } else if ( i == qr_ ) {
            if ( j == qt_ ) {
                if ( !is_hit_tr_ ) {
                    is_hit_tr_ = true;
                    hit_count_++;
                }
            } else if ( j == qb_ ) {
                if ( !is_hit_br_ ) {
                    is_hit_br_ = true;
                    hit_count_++;
                }
            } else {
                if ( !is_hit_r_ ) {
                    is_hit_r_ = true;
                    hit_count_++;
                }
            }
        } else {
            if ( j == qt_ ) {
                if ( !is_hit_t_ ) {
                    is_hit_t_ = true;
                    hit_count_++;
                }
            } else if ( j == qb_ ) {
                if ( !is_hit_b_ ) {
                    is_hit_b_ = true;
                    hit_count_++;
                }
            } else {
            }
        }
    }

    bool is_corner_hit() {
        if ( hit_count_ == 1 ) {
            return ( 
                is_hit_tl_ ||
                is_hit_tr_ || 
                is_hit_bl_ || 
                is_hit_br_ 
            );
        } else if ( hit_count_ == 2 ) {
            return ( 
                ( is_hit_tl_ && (is_hit_t_ || is_hit_l_) ) ||
                ( is_hit_tr_ && (is_hit_t_ || is_hit_r_) ) ||
                ( is_hit_bl_ && (is_hit_b_ || is_hit_l_) ) || 
                ( is_hit_br_ && (is_hit_b_ || is_hit_r_) ) 
            );
        } else if ( hit_count_ == 3 ) {
            return ( 
                ( is_hit_tl_ && is_hit_t_ && is_hit_l_ ) ||
                ( is_hit_tr_ && is_hit_t_ && is_hit_r_ ) ||
                ( is_hit_bl_ && is_hit_b_ && is_hit_l_ ) || 
                ( is_hit_br_ && is_hit_b_ && is_hit_r_ ) 
            );
        } else {
            return false;
        }
    }

    template <typename ObjT, typename MapT>
    mgc::physics::Vec2 calc_wall_pushback(
            const ObjT& obj,
            const mgc::collision::Hitbox& obj_hitbox,
            const MapT& map,
            CornerPushDirection push_dir
    ) {
        int32_t dx, dy;
        int8_t n_x, n_y;
        int16_t p_x, p_y;

        dx = MGC_MOD_CELL_LEN(obj.get_position().x + obj_hitbox.x0_ofs - map.get_position().x);
        dy = MGC_MOD_CELL_LEN(obj.get_position().y + obj_hitbox.y0_ofs - map.get_position().y);


        n_x = (is_hit_l_ ?    1 : 0) + (is_hit_tl_ ?    1 : 0) + (is_hit_bl_ ?    1 : 0)
            + (is_hit_r_ ? (-1) : 0) + (is_hit_tr_ ? (-1) : 0) + (is_hit_br_ ? (-1) : 0);
        n_y = (is_hit_t_ ?    1 : 0) + (is_hit_tl_ ?    1 : 0) + (is_hit_tr_ ?    1 : 0)
            + (is_hit_b_ ? (-1) : 0) + (is_hit_bl_ ? (-1) : 0) + (is_hit_br_ ? (-1) : 0);

        if ( n_x > 0 ) {
            p_x = MGC_CELL_LEN - dx;
        } else if ( n_x < 0 ) {
            p_x = MGC_MOD_CELL_LEN(dx + obj_hitbox.width)*-1;
        } else {
            p_x = 0;
        }

        if ( n_y > 0 ) {
            p_y = MGC_CELL_LEN - dy;
        } else if ( n_y < 0 ) {
            p_y = MGC_MOD_CELL_LEN(dy + obj_hitbox.height)*-1;
        } else {
            p_y = 0;
        }

        if ( this->is_corner_hit() ) {
            if ( MGC_ABS(p_x) > MGC_ABS(p_y) ) {
                p_x = 0;
            } else if ( MGC_ABS(p_x) < MGC_ABS(p_y) ) {
                p_y = 0;
            } else {
                switch (push_dir) {
                case CornerPushDirection::PreferX:
                    p_x = 0;
                    break;
                case CornerPushDirection::PreferY:
                    p_y = 0;
                    break;
                default:
                    break;
                }
            }
        }

        return mgc::physics::Vec2{ p_x, p_y };
    }

};


}// namespace collision
}// namespace mgc

#endif/*MGC_COLLISION_COLLISION_DETECTOR_HPP*/
