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
using ITalkflowEffectsT = mgc::control::talkflow::ITalkflowEffects;
using CameraT = mgc::camera::SimpleCameraFollower;
using ColBox2BoxT = mgc::collision::CollisionDetectorBoxToBox;
using ColBox2MapT = mgc::collision::CollisionDetectorBoxToMap;
using ColorT = mgc::graphics::Color;
using FramebufferT = mgc::graphics::Framebuffer;
using SpriteT = mgc::parts::BasicSprite;

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
struct GameContext;
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

namespace app::civilian {
struct Civilian;
}

namespace app::event {
struct Event;
}

namespace app::stage {
struct Stage;
}

namespace app {

void load_default_config(TalkflowControllerT& talkflow);

template<typename T>
struct ArrayViewer {
    T* data;
    size_t size;

    T* begin() { return data; }
    T* end() { return data + size; }

    const T* begin() const { return data; }
    const T* end() const { return data + size; }
};

}


#endif/*MGC_APP_COMMON_HPP*/

