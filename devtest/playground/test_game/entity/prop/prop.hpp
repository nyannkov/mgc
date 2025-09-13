#ifndef MGC_PROP_HPP
#define MGC_PROP_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"

namespace app {
namespace prop {

constexpr size_t PROP_HITBOX_COUNT_MAX = 1;

enum class PropType {
    Portal
};

struct Prop : mgc::entities::ActorImpl<Prop, PROP_HITBOX_COUNT_MAX> {
    
    virtual ~Prop() = default;

    virtual void spawn(const mgc::math::Vec2i& pos) = 0;
    virtual void despawn() = 0;
    
    PropType prop_type() const { return prop_type_; }

    template <typename Other>
    void on_hit_box_to_box_impl(
            const Other& other,
            const mgc::collision::BoxCollisionInfo& info
    ) { 
        if constexpr (std::is_same_v<Other, Player>) {
            on_player_hit(other, info);
        }
    }

protected:
    virtual void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) { }

    void set_prop_type(PropType type) {
        prop_type_ = type;
    }

private:
    PropType prop_type_ = PropType::Portal;
};

} // namespace prop
} // namespace app

#endif // MGC_PROP_HPP
