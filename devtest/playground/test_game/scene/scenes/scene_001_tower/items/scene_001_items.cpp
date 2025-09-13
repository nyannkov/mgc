#include "scene_001_items.hpp"

namespace app {

Scene001_Items::Scene001_Items(GameContext& ctx)
    : frame_timer_(ctx.frame_timer()),
      all_potion_ { item::Potion() },
      all_elixir_ { item::Elixir() },
      all_items_ { &all_potion_[0], &all_elixir_[0] } {
}

void Scene001_Items::init() {
    auto value = frame_timer_.now_ms() & 0xFF;
    if ( value < 30 ) {
        all_elixir_[0].spawn({MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(11)+8});
    } else {
        all_potion_[0].spawn({MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(11)+8});
    }
}


} // namespace app

