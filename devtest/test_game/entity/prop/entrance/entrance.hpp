#ifndef MGC_ENTRANCE_HPP
#define MGC_ENTRANCE_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "scene/scene_id.hpp"
#include "entity/prop/prop.hpp"

namespace app {
namespace prop {

enum class EntranceType {
    Type1,
    Type2
};

struct Entrance : Prop {
    Entrance();
    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn(const mgc::math::Vec2i& pos, EntranceType type);
    void despawn() override;
    void update_movement() override {};
    void update_animation() override {};

    void set_on_player_hit_scene_transition_request(
        SceneTransitionRequest request
    );

private:
    EntranceType type_ = EntranceType::Type1;

    void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) override;
};

} // namespace prop
} // namespace app

#endif // MGC_ENTRANCE_HPP
