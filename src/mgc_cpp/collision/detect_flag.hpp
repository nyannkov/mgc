/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2026 nyannkov
 */
#ifndef MGC_COLLISION_DETECT_FLAG_HPP
#define MGC_COLLISION_DETECT_FLAG_HPP

#include <type_traits>

namespace mgc {
namespace collision {

enum class DetectFlag : uint16_t {
    None = 0,
    Callback = 1 << 0,
};

inline DetectFlag operator|(DetectFlag a, DetectFlag b) noexcept {
    using T = std::underlying_type_t<DetectFlag>;
    return static_cast<DetectFlag>(
        static_cast<T>(a) | static_cast<T>(b)
    );
}

inline DetectFlag operator&(DetectFlag a, DetectFlag b) noexcept {
    using T = std::underlying_type_t<DetectFlag>;
    return static_cast<DetectFlag>(
        static_cast<T>(a) & static_cast<T>(b)
    );
}

inline DetectFlag& operator|=(DetectFlag& a, DetectFlag b) noexcept {
    a = a | b;
    return a;
}

inline bool has_flag(DetectFlag value, DetectFlag flag) noexcept {
    return (value & flag) != DetectFlag::None;
}

}// namespace collision
}// namespace mgc

#endif/*MGC_COLLISION_DETECT_FLAG_HPP*/
