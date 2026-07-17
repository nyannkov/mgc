#ifndef MGC_CARRIER_HPP
#define MGC_CARRIER_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "entity/player/player_hitbox_index.hpp"

namespace app {
namespace carrier {

struct Carrier : mgc::entities::TilemapImpl<Carrier> {

    virtual ~Carrier() = default;
    virtual void spawn(const mgc::math::Vec2i& pos) = 0;
    virtual void despawn() = 0;
    virtual void update_movement() = 0;
    virtual void resolve_movement() = 0;
    virtual void update_animation() = 0;

    auto delta() const { return delta_; }

protected:
    void set_delta(mgc::math::Vec2i d) { delta_ = d; }

private:
    mgc::math::Vec2i delta_{0, 0};
};

} // namespace carrier
} // namespace app

#endif // MGC_CARRIER_HPP
