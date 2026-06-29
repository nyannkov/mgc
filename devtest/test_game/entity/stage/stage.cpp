#include "stage.hpp"
#include "entity/player/player.hpp"
#include "resources/generated/map/map_tower_front_bg_back.h"
#include "resources/generated/map/map_tower_front_back_tower.h"
#include "resources/generated/map/map_tower_front_block.h"
#include "resources/generated/map/map_corridor_block.h"
#include "resources/generated/map/map_corridor_back.h"
#include "resources/generated/map/map_stage_1_1_block.h"
#include "resources/generated/map/map_stage_1_1_ladder.h"
#include "resources/generated/map/map_stage_1_1_one_way_block.h"
#include "resources/generated/map/map_stage_1_2_block.h"
#include "resources/generated/map/map_stage_1_2_ladder.h"
#include "resources/generated/map/map_stage_1_2_needle.h"
#include "resources/generated/map/map_stage_1_2_one_way_block.h"
#include "resources/generated/map/map_stage_1_3_block.h"
#include "resources/generated/map/map_stage_1_3_ladder.h"
#include "resources/generated/map/map_stage_1_3_needle.h"
#include "resources/generated/map/map_stage_1_3_one_way_block.h"
#include "resources/generated/map/map_stage_1_4_block.h"
#include "resources/generated/map/map_stage_1_4_ladder.h"
#include "resources/generated/map/map_stage_1_4_needle.h"
#include "resources/generated/map/map_stage_1_4_one_way_block.h"
#include "resources/generated/map/map_stage_1_5_block.h"
#include "resources/generated/map/map_stage_1_5_ladder.h"
#include "resources/generated/map/map_stage_1_5_needle.h"
#include "resources/generated/map/map_stage_1_5_one_way_block.h"
#include "resources/generated/map/map_stage_1_6_block.h"
#include "resources/generated/map/map_stage_1_6_ladder.h"
#include "resources/generated/map/map_stage_1_6_needle.h"
#include "resources/generated/map/map_stage_1_6_one_way_block.h"
#include "resources/generated/map/map_stage_1_7_block.h"
#include "resources/generated/map/map_stage_1_7_ladder.h"
#include "resources/generated/map/map_stage_1_7_needle.h"
#include "resources/generated/map/map_stage_1_7_one_way_block.h"
#include "resources/generated/map/map_stage_2_1_block.h"
#include "resources/generated/map/map_stage_2_1_ladder.h"
#include "resources/generated/map/map_stage_2_1_water.h"
#include "resources/generated/map/map_stage_2_1_needle.h"
#include "resources/generated/map/map_stage_2_1_one_way_block.h"
#include "resources/generated/map/map_stage_2_2_block.h"
#include "resources/generated/map/map_stage_2_2_ladder.h"
#include "resources/generated/map/map_stage_2_2_water.h"
#include "resources/generated/map/map_stage_2_2_needle.h"
#include "resources/generated/map/map_stage_2_2_one_way_block.h"
#include "resources/generated/map/map_stage_2_3_block.h"
#include "resources/generated/map/map_stage_2_3_ladder.h"
#include "resources/generated/map/map_stage_2_3_water.h"
#include "resources/generated/map/map_stage_2_3_needle.h"
#include "resources/generated/map/map_stage_2_3_one_way_block.h"
#include "resources/generated/map/map_stage_2_4_block.h"
#include "resources/generated/map/map_stage_2_4_ladder.h"
#include "resources/generated/map/map_stage_2_4_water.h"
#include "resources/generated/map/map_stage_2_4_needle.h"
#include "resources/generated/map/map_stage_2_4_one_way_block.h"
#include "resources/generated/map/map_stage_2_5_block.h"
#include "resources/generated/map/map_stage_2_5_ladder.h"
#include "resources/generated/map/map_stage_2_5_water.h"
#include "resources/generated/map/map_stage_2_5_needle.h"
#include "resources/generated/map/map_stage_2_5_one_way_block.h"
#include "resources/generated/map/map_stage_2_6_block.h"
#include "resources/generated/map/map_stage_2_6_ladder.h"
#include "resources/generated/map/map_stage_2_6_water.h"
#include "resources/generated/map/map_stage_2_6_needle.h"
#include "resources/generated/map/map_stage_2_6_one_way_block.h"
#include "resources/generated/map/map_stage_2_7_block.h"
#include "resources/generated/map/map_stage_2_7_ladder.h"
#include "resources/generated/map/map_stage_2_7_water.h"
#include "resources/generated/map/map_stage_2_7_needle.h"
#include "resources/generated/map/map_stage_2_7_one_way_block.h"
#include "resources/generated/map/map_stage_2_8_block.h"
#include "resources/generated/map/map_stage_2_8_ladder.h"
#include "resources/generated/map/map_stage_2_8_water.h"
#include "resources/generated/map/map_stage_2_8_needle.h"
#include "resources/generated/map/map_stage_2_8_one_way_block.h"
#include "resources/generated/map/map_stage_2_9_block.h"
#include "resources/generated/map/map_stage_2_9_ladder.h"
#include "resources/generated/map/map_stage_2_9_water.h"
#include "resources/generated/map/map_stage_2_9_needle.h"
#include "resources/generated/map/map_stage_2_9_one_way_block.h"
#include "resources/generated/map/map_stage_2_9_front.h"

#include "resources/generated/map/map_log_terminal_back.h"
#include "resources/generated/map/map_log_terminal_block.h"
#include "resources/generated/map/map_log_terminal_ladder.h"
#include "resources/generated/map/map_log_terminal_water.h"
#include "resources/generated/map/map_log_terminal_needle.h"
#include "resources/generated/map/map_log_terminal_one_way_block.h"

#include "resources/generated/map/map_shop_back_0.h"
#include "resources/generated/map/map_shop_back_1.h"
#include "resources/generated/map/map_shop_block.h"
#include "resources/generated/tileset/tileset_map_elements.h"
#include "resources/generated/tileset/tileset_shop_elements.h"

namespace app {
namespace stage {

Stage::Stage(const FrameTimerT& frame_timer)
    : pos_({0, 0}),
      stage_id_(StageId::None),
      back_0_(frame_timer),
      back_1_(frame_timer) { 
}

void Stage::resource_init() {
    block_.set_all_enabled(false);
    ladder_.set_all_enabled(false);
    one_way_block_.set_all_enabled(false);
    needle_.set_all_enabled(false);
    water_.set_all_enabled(false);
    back_0_.set_all_enabled(false);
    back_1_.set_all_enabled(false);
    front_.set_all_enabled(false);
}

void Stage::setup(StageId id) {
    stage_id_ = id;

    resource_init();
    back_0_.set_parallax_factor({1.0, 1.0});

    switch (id) {
    case StageId::None:
        break;

    case StageId::TowerFront:
        block_.set_maps(map_tower_front_block, tileset_map_elements, &map_tower_front_block);
        block_.set_all_enabled(true);

        back_0_.set_maps(map_tower_front_bg_back, tileset_map_elements);
        back_0_.set_all_enabled(true);
        back_0_.set_parallax_factor({0.3, 0.1});
        back_0_.set_tile_draw_hook_id(TileDrawHookId::TowerFront_LayerBack0);

        back_1_.set_maps(map_tower_front_back_tower, tileset_map_elements);
        back_1_.set_all_enabled(true);
        back_1_.set_tile_draw_hook_id(TileDrawHookId::None);
        break;

    case StageId::Corridor:
        block_.set_maps(map_corridor_block, tileset_map_elements, &map_corridor_block);
        block_.set_all_enabled(true);

        back_1_.set_maps(map_corridor_back, tileset_map_elements);
        back_1_.set_all_enabled(true);
        break;

    case StageId::Stage1_1:
        block_.set_maps(map_stage_1_1_block, tileset_map_elements, &map_stage_1_1_block);
        block_.set_all_enabled(true);

        one_way_block_.set_maps(map_stage_1_1_one_way_block, tileset_map_elements, &map_stage_1_1_one_way_block);
        one_way_block_.set_all_enabled(true);

        ladder_.set_maps(map_stage_1_1_ladder, tileset_map_elements, &map_stage_1_1_ladder);
        ladder_.set_all_enabled(true);
        break;

    case StageId::Stage1_2:
        block_.set_maps(map_stage_1_2_block, tileset_map_elements, &map_stage_1_2_block);
        block_.set_all_enabled(true);

        one_way_block_.set_maps(map_stage_1_2_one_way_block, tileset_map_elements, &map_stage_1_2_one_way_block);
        one_way_block_.set_all_enabled(true);

        ladder_.set_maps(map_stage_1_2_ladder, tileset_map_elements, &map_stage_1_2_ladder);
        ladder_.set_all_enabled(true);

        needle_.set_maps(map_stage_1_2_needle, tileset_map_elements, &map_stage_1_2_needle);
        needle_.set_all_enabled(true);
        break;

    case StageId::Stage1_3:
        block_.set_maps(map_stage_1_3_block, tileset_map_elements, &map_stage_1_3_block);
        block_.set_all_enabled(true);

        one_way_block_.set_maps(map_stage_1_3_one_way_block, tileset_map_elements, &map_stage_1_3_one_way_block);
        one_way_block_.set_all_enabled(true);

        ladder_.set_maps(map_stage_1_3_ladder, tileset_map_elements, &map_stage_1_3_ladder);
        ladder_.set_all_enabled(true);

        needle_.set_maps(map_stage_1_3_needle, tileset_map_elements, &map_stage_1_3_needle);
        needle_.set_all_enabled(true);
        break;

    case StageId::Stage1_4:
        block_.set_maps(map_stage_1_4_block, tileset_map_elements, &map_stage_1_4_block);
        block_.set_all_enabled(true);

        one_way_block_.set_maps(map_stage_1_4_one_way_block, tileset_map_elements, &map_stage_1_4_one_way_block);
        one_way_block_.set_all_enabled(true);

        ladder_.set_maps(map_stage_1_4_ladder, tileset_map_elements, &map_stage_1_4_ladder);
        ladder_.set_all_enabled(true);

        needle_.set_maps(map_stage_1_4_needle, tileset_map_elements, &map_stage_1_4_needle);
        needle_.set_all_enabled(true);
        break;

    case StageId::Stage1_5:
        block_.set_maps(map_stage_1_5_block, tileset_map_elements, &map_stage_1_5_block);
        block_.set_all_enabled(true);

        one_way_block_.set_maps(map_stage_1_5_one_way_block, tileset_map_elements, &map_stage_1_5_one_way_block);
        one_way_block_.set_all_enabled(true);

        ladder_.set_maps(map_stage_1_5_ladder, tileset_map_elements, &map_stage_1_5_ladder);
        ladder_.set_all_enabled(true);

        needle_.set_maps(map_stage_1_5_needle, tileset_map_elements, &map_stage_1_5_needle);
        needle_.set_all_enabled(true);
        break;

    case StageId::Stage1_6:
        block_.set_maps(map_stage_1_6_block, tileset_map_elements, &map_stage_1_6_block);
        block_.set_all_enabled(true);

        one_way_block_.set_maps(map_stage_1_6_one_way_block, tileset_map_elements, &map_stage_1_6_one_way_block);
        one_way_block_.set_all_enabled(true);

        ladder_.set_maps(map_stage_1_6_ladder, tileset_map_elements, &map_stage_1_6_ladder);
        ladder_.set_all_enabled(true);

        needle_.set_maps(map_stage_1_6_needle, tileset_map_elements, &map_stage_1_6_needle);
        needle_.set_all_enabled(true);
        break;

    case StageId::Stage1_7:
        block_.set_maps(map_stage_1_7_block, tileset_map_elements, &map_stage_1_7_block);
        block_.set_all_enabled(true);

        one_way_block_.set_maps(map_stage_1_7_one_way_block, tileset_map_elements, &map_stage_1_7_one_way_block);
        one_way_block_.set_all_enabled(true);

        ladder_.set_maps(map_stage_1_7_ladder, tileset_map_elements, &map_stage_1_7_ladder);
        ladder_.set_all_enabled(true);

        needle_.set_maps(map_stage_1_7_needle, tileset_map_elements, &map_stage_1_7_needle);
        needle_.set_all_enabled(true);
        break;

    case StageId::Stage2_1:
        block_.set_maps(map_stage_2_1_block, tileset_map_elements, &map_stage_2_1_block);
        block_.set_all_enabled(true);

        one_way_block_.set_maps(map_stage_2_1_one_way_block, tileset_map_elements, &map_stage_2_1_one_way_block);
        one_way_block_.set_all_enabled(true);

        ladder_.set_maps(map_stage_2_1_ladder, tileset_map_elements, &map_stage_2_1_ladder);
        ladder_.set_all_enabled(true);

        water_.set_maps(map_stage_2_1_water, tileset_map_elements, &map_stage_2_1_water);
        water_.set_all_enabled(true);

        needle_.set_maps(map_stage_2_1_needle, tileset_map_elements, &map_stage_2_1_needle);
        needle_.set_all_enabled(true);
        break;

    case StageId::Stage2_2:
        block_.set_maps(map_stage_2_2_block, tileset_map_elements, &map_stage_2_2_block);
        block_.set_all_enabled(true);

        one_way_block_.set_maps(map_stage_2_2_one_way_block, tileset_map_elements, &map_stage_2_2_one_way_block);
        one_way_block_.set_all_enabled(true);

        ladder_.set_maps(map_stage_2_2_ladder, tileset_map_elements, &map_stage_2_2_ladder);
        ladder_.set_all_enabled(true);

        water_.set_maps(map_stage_2_2_water, tileset_map_elements, &map_stage_2_2_water);
        water_.set_all_enabled(true);

        needle_.set_maps(map_stage_2_2_needle, tileset_map_elements, &map_stage_2_2_needle);
        needle_.set_all_enabled(true);
        break;

    case StageId::Stage2_3:
        block_.set_maps(map_stage_2_3_block, tileset_map_elements, &map_stage_2_3_block);
        block_.set_all_enabled(true);

        one_way_block_.set_maps(map_stage_2_3_one_way_block, tileset_map_elements, &map_stage_2_3_one_way_block);
        one_way_block_.set_all_enabled(true);

        ladder_.set_maps(map_stage_2_3_ladder, tileset_map_elements, &map_stage_2_3_ladder);
        ladder_.set_all_enabled(true);

        water_.set_maps(map_stage_2_3_water, tileset_map_elements, &map_stage_2_3_water);
        water_.set_all_enabled(true);

        needle_.set_maps(map_stage_2_3_needle, tileset_map_elements, &map_stage_2_3_needle);
        needle_.set_all_enabled(true);
        break;

    case StageId::Stage2_4:
        block_.set_maps(map_stage_2_4_block, tileset_map_elements, &map_stage_2_4_block);
        block_.set_all_enabled(true);

        one_way_block_.set_maps(map_stage_2_4_one_way_block, tileset_map_elements, &map_stage_2_4_one_way_block);
        one_way_block_.set_all_enabled(true);

        ladder_.set_maps(map_stage_2_4_ladder, tileset_map_elements, &map_stage_2_4_ladder);
        ladder_.set_all_enabled(true);

        water_.set_maps(map_stage_2_4_water, tileset_map_elements, &map_stage_2_4_water);
        water_.set_all_enabled(true);

        needle_.set_maps(map_stage_2_4_needle, tileset_map_elements, &map_stage_2_4_needle);
        needle_.set_all_enabled(true);
        break;

    case StageId::Stage2_5:
        block_.set_maps(map_stage_2_5_block, tileset_map_elements, &map_stage_2_5_block);
        block_.set_all_enabled(true);

        one_way_block_.set_maps(map_stage_2_5_one_way_block, tileset_map_elements, &map_stage_2_5_one_way_block);
        one_way_block_.set_all_enabled(true);

        ladder_.set_maps(map_stage_2_5_ladder, tileset_map_elements, &map_stage_2_5_ladder);
        ladder_.set_all_enabled(true);

        water_.set_maps(map_stage_2_5_water, tileset_map_elements, &map_stage_2_5_water);
        water_.set_all_enabled(true);

        needle_.set_maps(map_stage_2_5_needle, tileset_map_elements, &map_stage_2_5_needle);
        needle_.set_all_enabled(true);
        break;

    case StageId::Stage2_6:
        block_.set_maps(map_stage_2_6_block, tileset_map_elements, &map_stage_2_6_block);
        block_.set_all_enabled(true);

        one_way_block_.set_maps(map_stage_2_6_one_way_block, tileset_map_elements, &map_stage_2_6_one_way_block);
        one_way_block_.set_all_enabled(true);

        ladder_.set_maps(map_stage_2_6_ladder, tileset_map_elements, &map_stage_2_6_ladder);
        ladder_.set_all_enabled(true);

        water_.set_maps(map_stage_2_6_water, tileset_map_elements, &map_stage_2_6_water);
        water_.set_all_enabled(true);

        needle_.set_maps(map_stage_2_6_needle, tileset_map_elements, &map_stage_2_6_needle);
        needle_.set_all_enabled(true);
        break;

    case StageId::Stage2_7:
        block_.set_maps(map_stage_2_7_block, tileset_map_elements, &map_stage_2_7_block);
        block_.set_all_enabled(true);

        one_way_block_.set_maps(map_stage_2_7_one_way_block, tileset_map_elements, &map_stage_2_7_one_way_block);
        one_way_block_.set_all_enabled(true);

        ladder_.set_maps(map_stage_2_7_ladder, tileset_map_elements, &map_stage_2_7_ladder);
        ladder_.set_all_enabled(true);

        water_.set_maps(map_stage_2_7_water, tileset_map_elements, &map_stage_2_7_water);
        water_.set_all_enabled(true);

        needle_.set_maps(map_stage_2_7_needle, tileset_map_elements, &map_stage_2_7_needle);
        needle_.set_all_enabled(true);
        break;

    case StageId::Stage2_8:
        block_.set_maps(map_stage_2_8_block, tileset_map_elements, &map_stage_2_8_block);
        block_.set_all_enabled(true);

        one_way_block_.set_maps(map_stage_2_8_one_way_block, tileset_map_elements, &map_stage_2_8_one_way_block);
        one_way_block_.set_all_enabled(true);

        ladder_.set_maps(map_stage_2_8_ladder, tileset_map_elements, &map_stage_2_8_ladder);
        ladder_.set_all_enabled(true);

        water_.set_maps(map_stage_2_8_water, tileset_map_elements, &map_stage_2_8_water);
        water_.set_all_enabled(true);

        needle_.set_maps(map_stage_2_8_needle, tileset_map_elements, &map_stage_2_8_needle);
        needle_.set_all_enabled(true);
        break;

    case StageId::Stage2_9:
        block_.set_maps(map_stage_2_9_block, tileset_map_elements, &map_stage_2_9_block);
        block_.set_all_enabled(true);

        one_way_block_.set_maps(map_stage_2_9_one_way_block, tileset_map_elements, &map_stage_2_9_one_way_block);
        one_way_block_.set_all_enabled(true);

        ladder_.set_maps(map_stage_2_9_ladder, tileset_map_elements, &map_stage_2_9_ladder);
        ladder_.set_all_enabled(true);

        water_.set_maps(map_stage_2_9_water, tileset_map_elements, &map_stage_2_9_water);
        water_.set_all_enabled(true);

        needle_.set_maps(map_stage_2_9_needle, tileset_map_elements, &map_stage_2_9_needle);
        needle_.set_all_enabled(true);

        front_.set_maps(map_stage_2_9_front, tileset_map_elements, &map_stage_2_9_front);
        front_.set_all_enabled(true);
        break;

    case StageId::Shop:
        block_.set_maps(map_shop_block, tileset_shop_elements, &map_shop_block);
        block_.set_all_enabled(true);

        back_1_.set_maps(map_shop_back_1, tileset_shop_elements);
        back_1_.set_all_enabled(true);

        back_0_.set_maps(map_shop_back_0, tileset_shop_elements);
        back_0_.set_all_enabled(true);
        break;

    case StageId::LogTerminal:
        back_1_.set_maps(map_log_terminal_back, tileset_map_elements);
        back_1_.set_all_enabled(true);

        block_.set_maps(map_log_terminal_block, tileset_map_elements, &map_log_terminal_block);
        block_.set_all_enabled(true);

        one_way_block_.set_maps(map_log_terminal_one_way_block, tileset_map_elements, &map_log_terminal_one_way_block);
        one_way_block_.set_all_enabled(true);

        ladder_.set_maps(map_log_terminal_ladder, tileset_map_elements, &map_log_terminal_ladder);
        ladder_.set_all_enabled(true);

        water_.set_maps(map_log_terminal_water, tileset_map_elements, &map_log_terminal_water);
        water_.set_all_enabled(true);

        needle_.set_maps(map_log_terminal_needle, tileset_map_elements, &map_log_terminal_needle);
        needle_.set_all_enabled(true);
        break;

    default:
        break;
    }
}

void Stage::set_position(const mgc::math::Vec2i& position) {
    switch (stage_id_) {
    case StageId::TowerFront:
        block_.set_position(position);
        one_way_block_.set_position(position);
        ladder_.set_position(position);
        back_0_.set_position(position+mgc::math::Vec2i(0, MGC_CELL2PIXEL(3)));
        back_1_.set_position(position);
        front_.set_position(position);
        break;
    default:
        block_.set_position(position);
        one_way_block_.set_position(position);
        ladder_.set_position(position);
        back_0_.set_position(position);
        back_1_.set_position(position);
        front_.set_position(position);
        break;
    }
}


} // namespace stage
} // namespace app

