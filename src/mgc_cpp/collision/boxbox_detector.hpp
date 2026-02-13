/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2026 nyannkov
 */
#ifndef MGC_COLLISION_BOXBOX_DETECTOR_HPP
#define MGC_COLLISION_BOXBOX_DETECTOR_HPP

#include "mgc/detector/collision_boxbox.h"
#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/math/vec2.hpp"
#include "mgc_cpp/collision/hitbox.hpp"
#include "mgc_cpp/collision/collision_map.hpp"
#include "mgc_cpp/collision/map_collision_info.hpp"
#include "mgc_cpp/collision/detect_flag.hpp"
#include "mgc_cpp/collision/corner_push_direction.hpp"

namespace mgc {
namespace collision {

template <typename T>
struct BoxBoxArrayView {
    using raw_type = T;

    const T* data = nullptr;
    size_t count = 0;

    const T& get(size_t i) const { return data[i]; }
    size_t size() const noexcept { return count; }
    bool empty() const noexcept { return count == 0; }
};

template <typename T>
struct BoxBoxPtrArrayView {
    using raw_type = T;

    T* const* data = nullptr;
    size_t count = 0;

    const T& get(size_t i) const { return *data[i]; }
    size_t size() const noexcept { return count; }
    bool empty() const noexcept { return count == 0; }
};

struct BoxBoxResult {
    bool hit;
    bool is_fully_blocked;
    mgc::math::Vec2i pushback;
    mgc::math::Vec2i max_overlap;
};

// Query-only collision result (no side effects)
struct BoxBoxQueryResult {
    bool hit;
    mgc::math::Vec2i query_penetration;
};

struct BoxBoxDetectConfig {
    DetectFlag flags = DetectFlag::Callback;
    CornerPushDirection push_dir = CornerPushDirection::PreferY;
    float pushback_damping = 0.8;
};

using BoxMargin = mgc_aabb_margin_t;

struct BoxBoxDetector {
    
    template <typename T1, typename T2>
    static BoxBoxQueryResult query_pair(
        const T1& obj1,
        size_t hitbox_idx1,
        const BoxMargin& obj1_margin,
        const T2& obj2,
        size_t hitbox_idx2,
        const BoxMargin& obj2_margin
    ) {

        static_assert(mgc::entities::mixins::has_hitboxes<T1>::value, "T1 must have hitboxes()");
        static_assert(mgc::entities::mixins::has_hitboxes<T2>::value, "T2 must have hitboxes()");

        BoxBoxQueryResult result = { false, {0, 0} };

        const auto& obj1_hitboxes = obj1.hitboxes();
        if ( hitbox_idx1 >= obj1_hitboxes.size() ) {
            return result;
        }

        const auto& obj2_hitboxes = obj2.hitboxes();
        if ( hitbox_idx2 >= obj2_hitboxes.size() ) {
            return result;
        }

        const auto& h1 = obj1_hitboxes[hitbox_idx1];
        const auto& h2 = obj2_hitboxes[hitbox_idx2];

        if ( !h1.enabled() || !h2.enabled() ) {
            return result;
        }

        mgc_aabb_t base_aa, base_bb, aa, bb;
        collision_calc_aabb_from_hitbox(obj1.position().x, obj1.position().y, h1.c_ptr(), &base_aa);
        collision_expand_aabb_margin(&base_aa, &obj1_margin, &aa);

        collision_calc_aabb_from_hitbox(obj2.position().x, obj2.position().y, h2.c_ptr(), &base_bb);
        collision_expand_aabb_margin(&base_bb, &obj2_margin, &bb);

        bool r = collision_test_hit(&aa, &bb);
        
        if ( r ) {
            collision_calc_signed_overlap(
                &aa,
                &bb,
                &result.query_penetration.x, 
                &result.query_penetration.y
            );
            result.hit = true;
        }

        return result;
    }

    template <typename T1, typename T2>
    static bool detect_pair(
        T1& obj1,
        size_t hitbox_idx1,
        T2& obj2,
        size_t hitbox_idx2,
        DetectFlag flags = DetectFlag::Callback
    ) {
        static_assert(mgc::entities::mixins::has_hitboxes<T1>::value, "T1 must have hitboxes()");
        static_assert(mgc::entities::mixins::has_hitboxes<T2>::value, "T2 must have hitboxes()");

        const auto& obj1_hitboxes = obj1.hitboxes();
        if ( hitbox_idx1 >= obj1_hitboxes.size() ) {
            return false;
        }

        const auto& obj2_hitboxes = obj2.hitboxes();
        if ( hitbox_idx2 >= obj2_hitboxes.size() ) {
            return false;
        }

        const auto& h1 = obj1_hitboxes[hitbox_idx1];
        const auto& h2 = obj2_hitboxes[hitbox_idx2];

        if ( !h1.enabled() || !h2.enabled() ) {
            return false;
        }

        auto result = query_pair(
            obj1,
            hitbox_idx1,
            { 0, 0, 0, 0 },
            obj2,
            hitbox_idx2,
            { 0, 0, 0, 0 }
        );

        if ( result.hit ) {
            if ( has_flag(flags, DetectFlag::Callback) ) {
                if constexpr (std::is_base_of_v<mgc::entities::mixins::WithOnHitBoxToBoxResponse<T1>, T1>) {
                    auto signed_overlap = result.query_penetration;
                    const BoxCollisionInfo info = {h1, h2, hitbox_idx1, hitbox_idx2, {signed_overlap}};
                    obj1.on_hit_box_to_box(obj2, info);
                }
                if constexpr (std::is_base_of_v<mgc::entities::mixins::WithOnHitBoxToBoxResponse<T2>, T2>) {
                    auto signed_overlap = result.query_penetration * -1;
                    const BoxCollisionInfo info = {h2, h1, hitbox_idx2, hitbox_idx1, {signed_overlap}};
                    obj2.on_hit_box_to_box(obj1, info);
                }
            }
        }

        return result.hit;
    }

    template <typename T1, typename T2>
    static bool detect_pair(T1& obj1, T2& obj2, DetectFlag flags = DetectFlag::Callback) {

        const auto& obj1_hitboxes = obj1.hitboxes();
        const auto& obj2_hitboxes = obj2.hitboxes();

        bool r = false;

        for ( size_t h1_index = 0; h1_index < obj1_hitboxes.size(); h1_index++ ) {
            for ( size_t h2_index = 0; h2_index < obj2_hitboxes.size(); h2_index++ ) {
                if ( detect_pair(obj1, h1_index, obj2, h2_index, flags) ) {
                    r = true;
                }
            }
        }

        return r;
    }

    template <typename T1, typename T2>
    static bool detect_pair(T1& obj1, size_t hitbox_idx1, T2& obj2, DetectFlag flags = DetectFlag::Callback) {

        const auto& obj2_hitboxes = obj2.hitboxes();

        bool r = false;

        for ( size_t h2_index = 0; h2_index < obj2_hitboxes.size(); h2_index++ ) {
            if ( detect_pair(obj1, hitbox_idx1, obj2, h2_index, flags) ) {
                r = true;
            }
        }

        return r;
    }

    template <typename T1, typename T2>
    static bool detect_pair(T1& obj1, T2& obj2, size_t hitbox_idx2, DetectFlag flags = DetectFlag::Callback) {

        const auto& obj1_hitboxes = obj1.hitboxes();

        bool r = false;

        for ( size_t h1_index = 0; h1_index < obj1_hitboxes.size(); h1_index++ ) {
            if ( detect_pair(obj1, h1_index, obj2, hitbox_idx2, flags) ) {
                r = true;
            }
        }

        return r;
    }

    template <typename View, typename T1>
    static BoxBoxResult detect_primary_core(
        T1& obj1,
        size_t hitbox_idx1,
        mgc::math::Vec2f obj1_velocity,
        const View& obj2_view,
        size_t hitbox_idx2,
        const BoxBoxDetectConfig& config = {}
    ) {
        static_assert(mgc::entities::mixins::has_hitboxes<T1>::value, "T1 must have hitboxes()");

        using RawT2 = typename View::raw_type; 

        static_assert(mgc::entities::mixins::has_hitboxes<RawT2>::value, "RawT2 must have hitboxes()");

        BoxBoxResult result = {false, false, {0,0}, {0,0}};
        const auto& obj1_hitboxes = obj1.hitboxes();
        if ( hitbox_idx1 >= obj1_hitboxes.size() ) {
            return result;
        }

        const auto& h1 = obj1_hitboxes[hitbox_idx1];
        if ( !h1.enabled() ) {
            return result;
        }

        if ( obj2_view.empty() ) {
            return result;
        }

        mgc_collision_boxbox_t boxbox;
        collision_boxbox_init(
            &boxbox,
            h1.c_ptr(),
            config.pushback_damping
        );

        collision_boxbox_begin(
            &boxbox,
            obj1.position().x,
            obj1.position().y,
            obj1_velocity.x,
            obj1_velocity.y
        );

        for ( size_t i = 0; i < obj2_view.size(); ++i ) {
            const auto &obj2 = obj2_view.get(i);
            const auto& obj2_hitboxes = obj2.hitboxes();
            if ( hitbox_idx2 >= obj2_hitboxes.size() ) {
                continue;
            }

            const auto& h2 = obj2_hitboxes[hitbox_idx2];
            if ( !h2.enabled() ) {
                continue;
            }

            bool r = collision_boxbox_test_hit(&boxbox, h2.c_ptr(), obj2.position().x, obj2.position().y);

            if ( r ) {

                result.hit = true;

                if ( has_flag(config.flags, DetectFlag::Callback) ) {

                    if constexpr (std::is_base_of_v<mgc::entities::mixins::WithOnHitBoxToBoxResponse<T1>, T1>) {
                        const mgc::collision::BoxCollisionInfo info = {h1, h2, hitbox_idx1, hitbox_idx2};
                        obj1.on_hit_box_to_box(obj2, info);
                    }
                }

                collision_boxbox_accumulate_pushback(
                    &boxbox,
                    h2.c_ptr(),
                    obj2.position().x,
                    obj2.position().y,
                    to_c_dir(config.push_dir)
                );
            }
        }

        collision_boxbox_finalize(&boxbox);

        mgc_pushback_t pushback = {0, 0};
        collision_boxbox_get_pushback(&boxbox, &pushback);

        result.pushback.x = pushback.x;
        result.pushback.y = pushback.y;

        result.max_overlap.x = collision_boxbox_get_max_overlap_x(&boxbox);
        result.max_overlap.y = collision_boxbox_get_max_overlap_y(&boxbox);

        result.is_fully_blocked = collision_boxbox_is_fully_blocked(&boxbox);

        if ( has_flag(config.flags, DetectFlag::Callback) ) {

            if constexpr (std::is_base_of_v<mgc::entities::mixins::WithHandleBoxPushbackResult<T1>, T1>) {
                
                BoxPushbackInfo info = { {result.pushback}, {result.max_overlap}, result.is_fully_blocked };
                obj1.handle_box_pushback_result(info, obj2_view);
            }
        }

        return result;
    }

    template <typename T1, typename T2>
    static BoxBoxResult detect_primary_ptr(
        T1& obj1,
        size_t hitbox_idx1,
        mgc::math::Vec2f obj1_velocity,
        const BoxBoxPtrArrayView<T2>& obj2_view,
        size_t hitbox_idx2,
        const BoxBoxDetectConfig& config = {}
    ) {

        return detect_primary_core(
                obj1,
                hitbox_idx1,
                obj1_velocity,
                obj2_view,
                hitbox_idx2,
                config
        );
    }

    template <typename T1, typename T2>
    static BoxBoxResult detect_primary(
        T1& obj1,
        size_t hitbox_idx1,
        mgc::math::Vec2f obj1_velocity,
        const BoxBoxArrayView<T2>& obj2_view,
        size_t hitbox_idx2,
        const BoxBoxDetectConfig& config = {}
    ) {

        return detect_primary_core(
                obj1,
                hitbox_idx1,
                obj1_velocity,
                obj2_view,
                hitbox_idx2,
                config
        );
    }
};


}// namespace collision
}// namespace mgc

#endif/*MGC_COLLISION_BOXBOX_DETECTOR_HPP*/
