#ifndef MGC_SCENE_001_PROPS_HPP
#define MGC_SCENE_001_PROPS_HPP

#include "mgc_cpp/mgc.hpp"
#include "game_context/game_context.hpp"
#include "entity/prop/entrance/entrance.hpp"

namespace app {
struct Scene001_Props {

    static constexpr size_t EntranceCount = 2; 
    static constexpr size_t TotalCount = EntranceCount;

    explicit Scene001_Props(GameContext& ctx);

    void init();
    auto& all_entrance() { return all_entrance_; }
    auto& all() { return all_props_; }

private:
    GameContext& ctx_;
    std::array<prop::Entrance, EntranceCount> all_entrance_;
    std::array<prop::Prop*, TotalCount> all_props_;
};

} // namespace app

#endif // MGC_SCENE_001_PROPS_HPP

