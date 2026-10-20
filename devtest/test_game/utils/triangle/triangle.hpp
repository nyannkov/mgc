#ifndef MGC_PARTS_TRIANGLE_HPP
#define MGC_PARTS_TRIANGLE_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/math/vec2.hpp"
#include "mgc_cpp/features/resettable.hpp"
#include "mgc_cpp/features/has_id.hpp"
#include "mgc_cpp/features/has_position.hpp"
#include "mgc_cpp/features/has_parallax_factor.hpp"
#include "mgc_cpp/features/visible.hpp"
#include "mgc_cpp/features/drawable.hpp"
#include <algorithm>

namespace mgc {
namespace parts {

struct Triangle : mgc::features::Resettable,
                  mgc::features::HasId,
                  mgc::features::HasPosition<mgc::math::Vec2i>,
                  mgc::features::HasParallaxFactor,
                  mgc::features::Visible,
                  mgc::features::Drawable {

    Triangle() { reset(); }
    ~Triangle() = default;
    Triangle(const Triangle&) = delete;
    Triangle& operator=(const Triangle&) = delete;
    Triangle(Triangle&&) = default;
    Triangle& operator=(Triangle&&) = default;

    // [feature] Resettable
    void reset() override {
        id_ = 0;
        pos_ = delta0_ = delta1_ = delta2_ = { 0, 0 };
        para_factor_ = { 1, 1 };
        is_visible_ = true;
        point_array_ = nullptr;
        point_count_ = 0;
        color_ = MGC_COLOR_BLACK;
    }

    // [feature] HasId
    void set_id(mgc_id_t id) override { id_ = id; }
    mgc_id_t id() const override { return id_; }

    // [feature] HasPosition
    mgc::math::Vec2i position() const override { return pos_; }
    void set_position(const mgc::math::Vec2i& position) override { pos_ = position; }

    // [feature] HasParallaxFactor
    void set_parallax_factor(const mgc::graphics::ParallaxFactor &factor) override { para_factor_ = factor; }
    mgc::graphics::ParallaxFactor parallax_factor() const override { return para_factor_; }

    // [feature] Visible
    bool is_visible() const override { return is_visible_; }
    void set_visible(bool v) override { is_visible_ = v; }

    inline mgc_world_t cross_product(const mgc::math::Vec2i& a, const mgc::math::Vec2i& b, const mgc::math::Vec2i& p) const {
        return (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
    }

    // [feature] Drawable
    using mgc::features::Drawable::draw;
    bool draw(mgc::graphics::Framebuffer &fb, const mgc::math::Vec2i &cam_pos, const mgc::graphics::DrawOptions *options) const override {
        // 0: triangle, 1: camera
        mgc_world_t l0, l1;
        mgc_world_t r0, r1;
        mgc_world_t t0, t1;
        mgc_world_t b0, b1;

        if ( fb.data() == NULL ) {
            MGC_WARN("Invalid handler");
            return false;
        }
        if ( is_visible_ == false ) {
            MGC_INFO("Handler is not visible");
            return false;
        }

        mgc::math::Vec2i p0 = pos_ + delta0_;
        mgc::math::Vec2i p1 = pos_ + delta1_;
        mgc::math::Vec2i p2 = pos_ + delta2_;

        l0 = std::min({p0.x, p1.x, p2.x});
        r0 = std::max({p0.x, p1.x, p2.x});
        t0 = std::min({p0.y, p1.y, p2.y});
        b0 = std::max({p0.y, p1.y, p2.y});

        l1 = MGC_PARALLAX_SHIFT(cam_pos.x, para_factor_.x);
        t1 = MGC_PARALLAX_SHIFT(cam_pos.y, para_factor_.y);
        r1 = l1 + fb.width() - 1;
        b1 = t1 + fb.height()- 1;

        if ( (l0<=r1) && (l1<=r0) && (t0<=b1) && (t1<=b0) ) {
            int32_t x, y;
            int32_t x_s, y_s, x_e, y_e;

            x_s = (( l1 < l0 ) ? l0 : l1) - l0;
            x_e = (( r1 < r0 ) ? r1 : r0) - l0;
            y_s = (( t1 < t0 ) ? t0 : t1) - t0;
            y_e = (( b1 < b0 ) ? b1 : b0) - t0;

            for ( x = x_s; x <= x_e; x++ ) {
                for ( y = y_s; y <= y_e; y++ ) {
                    mgc::math::Vec2i p;
                    p.x = x + l0;
                    p.y = y + t0;

                    mgc_world_t w0 = cross_product(p1, p2, p);
                    mgc_world_t w1 = cross_product(p2, p0, p);
                    mgc_world_t w2 = cross_product(p0, p1, p);

                    if ( ( w0 >= 0 && w1 >= 0 && w2 >= 0 ) ||
                         ( w0 <= 0 && w1 <= 0 && w2 <= 0 )
                    ) {
                        size_t idx = MGC_GET_PIXELBUF_INDEX(x+l0-l1, y+t0-t1, fb.width(), fb.height());

                        if ( options && ( options->eff_flags & DRAW_EFFECT_ALPHA_BLEND ) ) {
                            mgc_color_t src = color_;
                            mgc_color_t dst = MGC_COLOR_SWAP(fb.data()[idx]); 
                            uint8_t a = options->alpha;
                            uint8_t sr = (src >> 11) & 0x1F; 
                            uint8_t sg = (src >>  5) & 0x3F; 
                            uint8_t sb =  src        & 0x1F; 
                            uint8_t dr = (dst >> 11) & 0x1F; 
                            uint8_t dg = (dst >>  5) & 0x3F; 
                            uint8_t db =  dst        & 0x1F; 
                            uint8_t r = (sr * a + dr * (255 - a) + 128) >> 8; 
                            uint8_t g = (sg * a + dg * (255 - a) + 128) >> 8; 
                            uint8_t b = (sb * a + db * (255 - a) + 128) >> 8; 
                            src = ((mgc_color_t)r<<11)|((mgc_color_t)g<<5)|((mgc_color_t)b<<0); 
                            fb.data()[idx] = MGC_COLOR_SWAP(src);
                        } else {
                            fb.data()[idx] = MGC_COLOR_SWAP(color_);
                        }
                    }
                }
            }
            return true;
        } else {
            return false;
        }
    }

    void set_delta0(const mgc::math::Vec2i& delta) { delta0_ = delta; }
    auto delta0() const { return delta0_; }

    void set_delta1(const mgc::math::Vec2i& delta) { delta1_ = delta; }
    auto delta1() const { return delta1_; }

    void set_delta2(const mgc::math::Vec2i& delta) { delta2_ = delta; }
    auto delta2() const { return delta2_; }

    void set_color(mgc_color_t color) { color_ = color; }
    mgc_color_t color() const { return color_; }

private:
    mgc_id_t id_;
    mgc::graphics::ParallaxFactor para_factor_;
    bool is_visible_;
    mgc::math::Vec2i pos_;
    mgc::math::Vec2i delta0_;
    mgc::math::Vec2i delta1_;
    mgc::math::Vec2i delta2_;

    const mgc::math::Vec2i *point_array_;
    size_t point_count_;
    mgc_color_t color_;
};

}// namespace parts
}// namespace mgc

#endif/*MGC_PARTS_TRIANGLE_HPP*/
