#ifndef MGC_APP_COMMON_HPP
#define MGC_APP_COMMON_HPP

#include <type_traits>
#include "mgc_cpp/mgc.hpp"
#include "platform/platform_conf.hpp"

namespace app {

constexpr uint16_t MAIN_FRAME_WIDTH = 224;
constexpr uint16_t MAIN_FRAME_HEIGHT = 192;

constexpr mgc_world_t TARGET_X_IN_CAMERA = MAIN_FRAME_WIDTH/2-8;
constexpr mgc_world_t TARGET_Y_IN_CAMERA = MAIN_FRAME_HEIGHT/2;

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

struct Player;
namespace attack { struct Attack; }
namespace enemy { struct Enemy; }
namespace item { struct Item; }
namespace prop { struct Prop; }
namespace block { struct Block; }
namespace carrier { struct Carrier; }
namespace civilian { struct Civilian; }
namespace event { struct Event; }
namespace stage { struct Stage; }

template <typename T, typename IndexT, size_t N>
constexpr T& at(std::array<T, N>& arr, IndexT index) noexcept {
    return arr[static_cast<size_t>(index)];
}

template <typename T, typename IndexT, size_t N>
constexpr const T& at(const std::array<T, N>& arr, IndexT index) noexcept {
    return arr[static_cast<size_t>(index)];
}

template<typename T>
struct ArrayViewer {
    T* data;
    size_t size;

    T* begin() { return data; }
    T* end() { return data + size; }

    const T* begin() const { return data; }
    const T* end() const { return data + size; }
};

void fill_color_with_alpha(
    FramebufferT& fb,
    ColorT fill_color,
    uint8_t alpha
);

} // namespace app

#endif/*MGC_APP_COMMON_HPP*/

