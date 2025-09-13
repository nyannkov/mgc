#ifndef MGC_SCENE_001_ITEMS_HPP
#define MGC_SCENE_001_ITEMS_HPP

#include "mgc_cpp/mgc.hpp"
#include "game_context/game_context.hpp"
#include "entity/item/potion/potion.hpp"
#include "entity/item/elixir/elixir.hpp"

namespace app {
struct Scene001_Items {

    static constexpr size_t PotionCount = 1; 
    static constexpr size_t ElixirCount = 1; 
    static constexpr size_t TotalCount = PotionCount + ElixirCount;

    explicit Scene001_Items(GameContext& ctx);

    void init();
    auto& all_potion() { return all_potion_; }
    auto& all_elixir() { return all_elixir_; }
    auto& all() { return all_items_; }

private:
    const FrameTimerT& frame_timer_;
    std::array<item::Potion, PotionCount> all_potion_;
    std::array<item::Elixir, ElixirCount> all_elixir_;
    std::array<item::Item*, TotalCount> all_items_;
};

} // namespace app

#endif // MGC_SCENE_001_ITEMS_HPP

