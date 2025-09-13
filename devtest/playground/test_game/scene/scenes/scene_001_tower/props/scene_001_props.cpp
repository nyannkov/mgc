#include "scene_001_props.hpp"

namespace app {

Scene001_Props::Scene001_Props(GameContext& ctx)
    : ctx_(ctx),
      all_entrance_ { 
        prop::Entrance(ctx.stage_info(), ctx.gamepad()),
        prop::Entrance(ctx.stage_info(), ctx.gamepad())
      },
      all_props_ { 
        &all_entrance_[0],
        &all_entrance_[1] 
      } {
}

void Scene001_Props::init() {

    all_entrance_[0].spawn({MGC_CELL2PIXEL(18), MGC_CELL2PIXEL(5)}, prop::EntranceDirection::Up, ctx_);
    all_entrance_[0].set_next_scene(SceneId::Id_001);

    all_entrance_[1].spawn({MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(38)}, prop::EntranceDirection::Down, ctx_);
    all_entrance_[1].set_next_scene(SceneId::Id_001);
}


} // namespace app

