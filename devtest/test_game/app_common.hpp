#ifndef MGC_APP_COMMON_HPP
#define MGC_APP_COMMON_HPP

#include <type_traits>
#include "mgc_cpp/mgc.hpp"
#include "platform/platform_conf.hpp"

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
using ColBox2BoxT = mgc::collision::BoxBoxDetector;
using ColBox2MapT = mgc::collision::BoxMapDetector;
using ColorT = mgc::graphics::Color;
using FramebufferT = mgc::graphics::Framebuffer;
using SpriteT = mgc::parts::BasicSprite;
using LabelT = mgc::parts::BasicLabel;
using DialogueboxT = mgc::parts::BasicDialoguebox;
using TilegridT = mgc::parts::BasicTilegrid;

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
struct EquipmentInfo;
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

namespace app::block {
struct Block;
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
bool point_in_box(
    int32_t x, int32_t y,
    int32_t l, int32_t r,
    int32_t t, int32_t b
);

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

