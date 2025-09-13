/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_COLLISION_COLLISION_DETECTOR_HPP
#define MGC_COLLISION_COLLISION_DETECTOR_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/math/vec2.hpp"
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

        for ( size_t h1_index = 0; h1_index < obj1_hitboxes.size(); h1_index++ ) {
            auto& h1 = obj1_hitboxes[h1_index];
            if ( !h1.enabled ) {
                continue;
            }
            for ( size_t h2_index = 0; h2_index < obj2_hitboxes.size(); h2_index++ ) {
                auto& h2 = obj2_hitboxes[h2_index];
                if ( !h2.enabled ) {
                    continue;
                }
                int32_t l1 = obj1.position().x + h1.offset.x;
                int32_t r1 = l1 + h1.size.width() - 1;
                int32_t t1 = obj1.position().y + h1.offset.y;
                int32_t b1 = t1 + h1.size.height() - 1;

                int32_t l2 = obj2.position().x + h2.offset.x;
                int32_t r2 = l2 + h2.size.width() - 1;
                int32_t t2 = obj2.position().y + h2.offset.y;
                int32_t b2 = t2 + h2.size.height() - 1;

                if ((l1 <= r2) && (l2 <= r1) && (t1 <= b2) && (t2 <= b1)) {
                    const mgc::collision::BoxCollisionInfo info1 = {h1, h2, h1_index, h2_index};
                    obj1.on_hit_box_to_box(obj2, info1);

                    const mgc::collision::BoxCollisionInfo info2 = {h2, h1, h2_index, h1_index};
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
    CollisionDetectorBoxToMap(const CollisionDetectorBoxToMap&) = delete;
    CollisionDetectorBoxToMap& operator=(const CollisionDetectorBoxToMap&) = delete;
    CollisionDetectorBoxToMap(CollisionDetectorBoxToMap&&) = default;
    CollisionDetectorBoxToMap& operator=(CollisionDetectorBoxToMap&&) = default;

    template <typename ObjT, typename MapT>
    void detect(ObjT& obj, MapT& map, CornerPushDirection push_dir = CornerPushDirection::None) {
        static_assert(std::is_base_of_v<mgc::entities::mixins::WithHitboxes<ObjT, ObjT::HitboxCount>, ObjT>,
                      "ObjT must inherit from WithHitboxes<ObjT, N>");

        static_assert(std::is_base_of_v<mgc::entities::mixins::WithOnHitBoxToMapResponse<ObjT>, ObjT>,
                      "ObjT must inherit from WithOnHitBoxToMapResponse<ObjT>");

        static_assert(std::is_base_of_v<mgc::entities::mixins::WithHandleMapPushbackResult<ObjT>, ObjT>,
                      "ObjT must inherit from WithHandleMapPushbackResult<ObjT>");

        static_assert(std::is_base_of_v<mgc::entities::mixins::WithCollisionMap<MapT>, MapT>,
                      "MapT must inherit from WithCollisionMap<MapT>");

        static_assert(std::is_base_of_v<mgc::entities::mixins::WithOnHitBoxToMapResponse<MapT>, MapT>,
                      "MapT must inherit from WithOnHitBoxToMapResponse<MapT>");

        static_assert(std::is_base_of_v<mgc::entities::mixins::WithHandleMapPushbackResult<MapT>, MapT>,
                      "MapT must inherit from WithHandleMapPushbackResult<MapT>");

        if ( !map.collision_enabled() ) {
            return;
        }
        auto* collision_map = map.collision_map();
        if ( !collision_map ) {
            return;
        }
        const auto& obj_hitboxes = obj.hitboxes();

        this->init();

        for ( size_t h_index = 0; h_index < obj_hitboxes.size(); h_index++ ) {
            auto& h = obj_hitboxes[h_index];
            if ( !h.enabled ) {
                continue;
            }
            ql_ = MGC_DIV_CELL_LEN((int32_t)obj.position().x + h.offset.x - map.position().x);
            qr_ = MGC_DIV_CELL_LEN((int32_t)obj.position().x + h.offset.x + h.size.width() - 1 - map.position().x);
            qt_ = MGC_DIV_CELL_LEN((int32_t)obj.position().y + h.offset.y - map.position().y);
            qb_ = MGC_DIV_CELL_LEN((int32_t)obj.position().y + h.offset.y + h.size.height() - 1 - map.position().y);
            if ( ql_ < 0 ) ql_ = 0;
            if ( qt_ < 0 ) qt_ = 0;
            if ( collision_map->map_width <= qr_ ) qr_ = collision_map->map_width - 1;
            if ( collision_map->map_height <= qb_ ) qb_ = collision_map->map_height - 1;

            for ( uint16_t j = qt_ ; j <= qb_; j++ ) {
                for ( uint16_t i = ql_; i <= qr_; i++ ) {
                    uint8_t map_cell_value = map_get_map_cell_value(collision_map, i, j);
                    if ( (map_cell_value  & 0x80) != 0 ) {
                        this->update_hit_flags_and_counter(i, j);

                        const mgc::collision::MapCollisionInfo info = {h, h_index, map_cell_value, j, i};
                        obj.on_hit_box_to_map(obj, map, info);
                        map.on_hit_box_to_map(obj, map, info);
                    }
                }
            }

            if ( hit_count_ > 0 ) {
                mgc::math::Vec2i pushback = calc_wall_pushback(obj, h, map, push_dir);
                const mgc::collision::MapPushbackInfo info = { h, h_index, pushback };
                obj.handle_map_pushback_result(obj, map, info);
                map.handle_map_pushback_result(obj, map, info);
            }
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

    bool is_corner_hit() const {
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
    mgc::math::Vec2i calc_wall_pushback(
            const ObjT& obj,
            const mgc::collision::Hitbox& obj_hitbox,
            const MapT& map,
            CornerPushDirection push_dir
    ) const {
        int32_t dx, dy;
        int8_t n_x, n_y;
        int16_t p_x, p_y;

        dx = MGC_MOD_CELL_LEN(obj.position().x + obj_hitbox.offset.x - map.position().x);
        dy = MGC_MOD_CELL_LEN(obj.position().y + obj_hitbox.offset.y - map.position().y);


        n_x = (is_hit_l_ ?    1 : 0) + (is_hit_tl_ ?    1 : 0) + (is_hit_bl_ ?    1 : 0)
            + (is_hit_r_ ? (-1) : 0) + (is_hit_tr_ ? (-1) : 0) + (is_hit_br_ ? (-1) : 0);
        n_y = (is_hit_t_ ?    1 : 0) + (is_hit_tl_ ?    1 : 0) + (is_hit_tr_ ?    1 : 0)
            + (is_hit_b_ ? (-1) : 0) + (is_hit_bl_ ? (-1) : 0) + (is_hit_br_ ? (-1) : 0);

        if ( n_x > 0 ) {
            p_x = MGC_CELL_LEN - dx;
        } else if ( n_x < 0 ) {
            p_x = MGC_MOD_CELL_LEN(dx + obj_hitbox.size.width())*-1;
        } else {
            p_x = 0;
        }

        if ( n_y > 0 ) {
            p_y = MGC_CELL_LEN - dy;
        } else if ( n_y < 0 ) {
            p_y = MGC_MOD_CELL_LEN(dy + obj_hitbox.size.height())*-1;
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
                    p_y = 0;
                    break;
                case CornerPushDirection::PreferY:
                    p_x = 0;
                    break;
                default:
                    break;
                }
            }
        }

        return mgc::math::Vec2i(p_x, p_y);
    }

};


}// namespace collision
}// namespace mgc

#endif/*MGC_COLLISION_COLLISION_DETECTOR_HPP*/
