#ifndef MGC_STAGE_HPP
#define MGC_STAGE_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "layer/layer_block.hpp"
#include "layer/layer_one_way_block.hpp"
#include "layer/layer_needle.hpp"
#include "layer/layer_ladder.hpp"
#include "layer/layer_water.hpp"
#include "layer/layer_back.hpp"
#include "layer/layer_front.hpp"
#include "stage_id.hpp"

namespace app {
struct Player;

namespace stage {
    
struct Stage {

    explicit Stage(const FrameTimerT& frame_timer);
    ~Stage() = default;

    void setup(StageId id);
    void set_position(const mgc::math::Vec2i& position);

    template <typename T>
    void detect_hit(T& obj) {
        ColBox2MapT::detect(obj, block_);
        ColBox2MapT::detect(obj, one_way_block_);
        ColBox2MapT::detect(obj, ladder_);
        ColBox2MapT::detect(obj, water_);
        ColBox2MapT::detect(obj, needle_);
    }

    template <typename T>
    void detect_hit(T& obj, size_t idx) {
        ColBox2MapT::detect(obj, idx, block_);
        ColBox2MapT::detect(obj, idx, one_way_block_);
        ColBox2MapT::detect(obj, idx, ladder_);
        ColBox2MapT::detect(obj, idx, water_);
        ColBox2MapT::detect(obj, idx, needle_);
    }

    mgc::math::Vec2i position() const { return pos_; }
    StageId stage_id() const { return stage_id_; }

    bool draw(
        mgc::graphics::Framebuffer &fb,
        const mgc::math::Vec2i &cam_pos
    ) {
        bool r = false;
        r |= back_0_.draw(fb, cam_pos);
        r |= back_1_.draw(fb, cam_pos);
        r |= water_.draw(fb, cam_pos);
        r |= block_.draw(fb, cam_pos);
        r |= ladder_.draw(fb, cam_pos);
        r |= one_way_block_.draw(fb, cam_pos);
        r |= needle_.draw(fb, cam_pos);
        return r;
    }

    bool draw_after(
        mgc::graphics::Framebuffer &fb,
        const mgc::math::Vec2i &cam_pos
    ) {
        bool r = false;
        r = front_.draw(fb, cam_pos);
        return r;
    }

    void set_ladder_enabled(bool enabled) {
        ladder_.set_all_enabled(enabled);
    }

    void set_water_enabled(bool enabled) {
        water_.set_all_enabled(enabled);
    }

    void set_water_position(mgc::math::Vec2i pos) {
        water_.set_position(pos);
    }

    auto water_position() const {
        return water_.position();
    }

private:
    mgc::math::Vec2i pos_;
    StageId stage_id_;
    LayerBlock block_;
    LayerOneWayBlock one_way_block_;
    LayerNeedle needle_;
    LayerLadder ladder_;
    LayerWater water_;
    LayerBack back_0_;
    LayerBack back_1_;
    LayerFront front_;

    void resource_init();
};

} // namespace stage
} // namespace app

#endif // MGC_STAGE_HPP
