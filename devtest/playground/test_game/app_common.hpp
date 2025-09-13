#ifndef MGC_APP_COMMON_HPP
#define MGC_APP_COMMON_HPP

#include <type_traits>
#include "mgc_cpp/mgc.hpp"
#include "platform_conf.hpp"

namespace app {

// Alias
using BTreeControllerT = mgc::control::btree::BTreeController<FrameTimerT>;
using IBTListenerT =  mgc::control::btree::IBTreeListener<BTreeControllerT>;
using AnimControllerT = mgc::control::anim::AnimController<FrameTimerT>;
using StopwatchT = mgc::utils::Stopwatch<FrameTimerT>;
using BlinkAnimatorT = mgc::utils::BlinkAnimator<FrameTimerT>;
using TalkflowControllerT = mgc::control::talkflow::DefaultTalkflowController;
using ITalkflowListenerT = mgc::control::talkflow::ITalkflowListener;

template <typename T, typename IndexT, size_t N>
constexpr T& at(std::array<T, N>& arr, IndexT index) noexcept {
    return arr[static_cast<size_t>(index)];
}

template <typename T, typename IndexT, size_t N>
constexpr const T& at(const std::array<T, N>& arr, IndexT index) noexcept {
    return arr[static_cast<size_t>(index)];
}

} // namespace app

namespace app {
struct Player;
struct Attack;
struct CollisionTileLayer;
struct TileLayer;
struct GameContext;
struct TriggerInfo;//TODO
}

namespace app::enemy {
struct Enemy;
}

namespace app::item {
struct Item;
}

namespace app::prop {
struct Prop;
}

#endif/*MGC_APP_COMMON_HPP*/

