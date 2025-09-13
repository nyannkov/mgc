#ifndef MGC_SCENE_001_ENEMIES_HPP
#define MGC_SCENE_001_ENEMIES_HPP

#include "mgc_cpp/mgc.hpp"
#include "game_context/game_context.hpp"
#include "entity/enemy/skyfish/skyfish.hpp"

namespace app {
struct Scene001_Enemies {

    static constexpr size_t SkyFishCount = 5; 
    static constexpr size_t TotalCount = SkyFishCount;

    explicit Scene001_Enemies(GameContext& ctx);

    void init();
    auto& all_skyfish() { return all_skyfish_; }
    auto& all() { return all_enemies_; }

private:
    std::array<enemy::SkyFish, SkyFishCount> all_skyfish_;
    std::array<enemy::Enemy*, TotalCount> all_enemies_;
};

} // namespace app

#endif // MGC_SCENE_001_ENEMIES_HPP

