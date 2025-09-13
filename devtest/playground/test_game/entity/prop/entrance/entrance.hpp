#ifndef MGC_ENTRANCE_HPP
#define MGC_ENTRANCE_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "entity/prop/prop.hpp"
#include "game_context/stage_info.hpp"

namespace app {
namespace prop {

enum class EntranceDirection {
    Up,
    Down
};

struct Entrance : Prop {
    Entrance(StageInfo& stage_info, const GamepadT& gamepad);
    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn(const mgc::math::Vec2i& pos, EntranceDirection dir, GameContext& ctx);
    void despawn() override;

    void set_next_scene(SceneId id) { next_scene_ = id; }
private:
    StageInfo& stage_info_;
    const GamepadT& gamepad_;
    SceneId next_scene_ = SceneId::Id_000;
    EntranceDirection direction_;

    void on_player_hit(
        const Player& player,
        const mgc::collision::BoxCollisionInfo& info
    ) override;
};

} // namespace prop
} // namespace app

#endif // MGC_ENTRANCE_HPP
