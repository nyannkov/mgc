#ifndef MGC_STAGE_HPP
#define MGC_STAGE_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "layer/layer_block.hpp"
#include "layer/layer_one_way_block.hpp"
#include "layer/layer_ladder.hpp"
#include "layer/layer_back.hpp"
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
        col_detector_.detect(obj, block_);
        col_detector_.detect(obj, one_way_block_);
        col_detector_.detect(obj, ladder_);
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
        r |= block_.draw(fb, cam_pos);
        r |= ladder_.draw(fb, cam_pos);
        r |= one_way_block_.draw(fb, cam_pos);
        return r;
    }

private:
    mgc::math::Vec2i pos_;
    StageId stage_id_;
    LayerBlock block_;
    LayerOneWayBlock one_way_block_;
    LayerLadder ladder_;
    LayerBack back_0_;
    LayerBack back_1_;
    mgc::collision::CollisionDetectorBoxToMap col_detector_;
};

} // namespace stage
} // namespace app

#endif // MGC_STAGE_HPP
