#include "scene_001_enemies.hpp"

namespace app {

Scene001_Enemies::Scene001_Enemies(GameContext& ctx)
    : all_skyfish_ {
          enemy::SkyFish(ctx.frame_timer(), ctx.sound_controller(), ctx.player(), ctx.player_stats()),
          enemy::SkyFish(ctx.frame_timer(), ctx.sound_controller(), ctx.player(), ctx.player_stats()),
          enemy::SkyFish(ctx.frame_timer(), ctx.sound_controller(), ctx.player(), ctx.player_stats()),
          enemy::SkyFish(ctx.frame_timer(), ctx.sound_controller(), ctx.player(), ctx.player_stats()),
          enemy::SkyFish(ctx.frame_timer(), ctx.sound_controller(), ctx.player(), ctx.player_stats())
      },
      all_enemies_ {
        &all_skyfish_[0],
        &all_skyfish_[1],
        &all_skyfish_[2],
        &all_skyfish_[3],
        &all_skyfish_[4]
      } {
}

void Scene001_Enemies::init() {
    all_skyfish_[0].spawn({MGC_CELL2PIXEL(6), MGC_CELL2PIXEL(32)}, true);
    all_skyfish_[1].spawn({MGC_CELL2PIXEL(15), MGC_CELL2PIXEL(25)}, false);
    all_skyfish_[2].spawn({MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(20)}, true);
    all_skyfish_[3].spawn({MGC_CELL2PIXEL(18), MGC_CELL2PIXEL(13)}, false);
    all_skyfish_[4].spawn({MGC_CELL2PIXEL(5), MGC_CELL2PIXEL(5)}, true);
}


} // namespace app

