/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2026 nyannkov
 */
#ifndef MGC_COLLISION_BOXMAP_DETECTOR_HPP
#define MGC_COLLISION_BOXMAP_DETECTOR_HPP

#include "mgc/detector/collision_boxmap.h"
#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/math/vec2.hpp"
#include "mgc_cpp/collision/hitbox.hpp"
#include "mgc_cpp/collision/collision_map.hpp"
#include "mgc_cpp/collision/map_collision_info.hpp"
#include "mgc_cpp/collision/detect_flag.hpp"
#include "mgc_cpp/collision/corner_push_direction.hpp"

namespace mgc {
namespace collision {

struct BoxMapResult {
    bool hit;
    bool is_fully_blocked;
    mgc::math::Vec2i pushback;
};

struct BoxMapDetectConfig {
    DetectFlag flags = DetectFlag::Callback;
    CornerPushDirection push_dir = CornerPushDirection::None;
};

struct BoxMapDetector {

    template <typename ObjT, typename MapT>
    static BoxMapResult detect(
        ObjT& obj, 
        size_t hitbox_idx,
        MapT& map,
        const BoxMapDetectConfig& config = {}) {

        static_assert(mgc::entities::mixins::has_hitboxes<ObjT>::value, "ObjT must have hitboxes()");
        static_assert(mgc::entities::mixins::has_collision_map<MapT>::value, "MapT must have collision_map()");

        BoxMapResult result = {false, false, {0, 0}};

        if ( !map.collision_enabled() ) {
            return result;
        }
        const auto* collision_map = map.collision_map();
        if ( !collision_map ) {
            return result;
        }
        const auto& obj_hitboxes = obj.hitboxes();
        if ( hitbox_idx >= obj_hitboxes.size() ) {
            return result;
        }
        const auto& h = obj_hitboxes[hitbox_idx];
        if ( !h.enabled() ) {
            return result;
        }

        mgc_collision_boxmap_t boxmap;
        collision_boxmap_init(
            &boxmap,
            h.c_ptr(), 
            obj.position().x,
            obj.position().y,
            collision_map,
            map.position().x,
            map.position().y
        );

        collision_boxmap_begin(&boxmap);

        mgc_map_range_t range;

        if ( !collision_boxmap_calc_map_range(&boxmap, &range) ) {
            return result;
        }

        for ( uint16_t row = range.row_min;; ++row ) {
            for ( uint16_t col = range.col_min;; ++col ) {
                uint8_t map_cell_value = 0;
                bool r = collision_boxmap_test_hit_cell(&boxmap, row, col, &range, &map_cell_value);
                if ( r ) {
                    result.hit = true;
                    if ( has_flag(config.flags, DetectFlag::Callback) ) {
                        const mgc::collision::MapCollisionInfo info = {
                            h, hitbox_idx, map_cell_value, row, col 
                        };

                        obj.on_hit_box_to_map(obj, map, info);
                        map.on_hit_box_to_map(obj, map, info);
                    }
                }
                if ( col == range.col_max ) break;
            }
            if ( row == range.row_max ) break;
        }

        if ( result.hit ) {

            mgc_pushback_t pushback = {0, 0};
            collision_boxmap_calc_pushback(
                &boxmap,
                to_c_dir(config.push_dir),
                &pushback
            );

            result.pushback.x = pushback.x;
            result.pushback.y = pushback.y;
            result.is_fully_blocked = collision_boxmap_is_fully_blocked(&boxmap);

            if ( has_flag(config.flags, DetectFlag::Callback) ) {

                MapPushbackInfo info = { h, hitbox_idx, {result.pushback}, result.is_fully_blocked };

                obj.handle_map_pushback_result(obj, map, info);
                map.handle_map_pushback_result(obj, map, info);
            }
        }

        return result;
    }
};


}// namespace collision
}// namespace mgc

#endif/*MGC_COLLISION_BOXMAP_DETECTOR_HPP*/
