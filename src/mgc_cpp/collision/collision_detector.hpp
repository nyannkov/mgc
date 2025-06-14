/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_COLLISION_COLLISION_DETECTOR_HPP
#define MGC_COLLISION_COLLISION_DETECTOR_HPP

#include "mgc/components/hitbox.h"

namespace mgc {
namespace collision {


template <typename T1, typename T2>
void check_collision_box_to_box(T1& obj1, T2& obj2) {
    static_assert(std::is_base_of<mgc::entities::mixins::WithHitboxes<T1, T1::HitboxCount>, T1>::value,
                  "T1 must inherit from WithHitboxes<T1, N>");

    static_assert(std::is_base_of<mgc::entities::mixins::WithOnHitHitboxResponse<T1>, T1>::value,
                  "T1 must inherit from WithOnHitHitboxResponse<T1>");

    static_assert(std::is_base_of<mgc::entities::mixins::WithHitboxes<T2, T2::HitboxCount>, T2>::value,
                  "T2 must inherit from WithHitboxes<T2, N>");

    static_assert(std::is_base_of<mgc::entities::mixins::WithOnHitHitboxResponse<T2>, T2>::value,
                  "T2 must inherit from WithOnHitHitboxResponse<T2>");

    const auto& obj1_hitboxes = obj1.hitboxes();
    const auto& obj2_hitboxes = obj2.hitboxes();

    for ( auto& h1 : obj1_hitboxes ) {
        for ( auto& h2 : obj2_hitboxes ) {
            if ( hitbox_is_enabled(&h1) && hitbox_is_enabled(&h2) ) {
                int32_t l1 = obj1.get_position().x + h1.x0_ofs;
                int32_t r1 = l1 + h1.width - 1;
                int32_t t1 = obj1.get_position().y + h1.y0_ofs;
                int32_t b1 = t1 + h1.height - 1;

                int32_t l2 = obj2.get_position().x + h2.x0_ofs;
                int32_t r2 = l2 + h2.width - 1;
                int32_t t2 = obj2.get_position().y + h2.y0_ofs;
                int32_t b2 = t2 + h2.height - 1;

                if ((l1 <= r2) && (l2 <= r1) && (t1 <= b2) && (t2 <= b1)) {
                    obj1.on_hit(obj2);
                    obj2.on_hit(obj1);
                }
            }
        }
    }
}


}// namespace collision
}// namespace mgc

#endif/*MGC_COLLISION_COLLISION_DETECTOR_HPP*/
