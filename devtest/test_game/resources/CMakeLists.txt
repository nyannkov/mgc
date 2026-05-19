cmake_minimum_required(VERSION 3.14)

project(resources C)

include(resource_gen.cmake)

add_library(resources_config INTERFACE)
target_include_directories(resources_config INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

set(STAGE_MAP_TOWER_FONT_CSVS
    "${RESOURCE_DIR}/stage/map/tower_front/csv/map_tower_front_back_tower.csv"
    "${RESOURCE_DIR}/stage/map/tower_front/csv/map_tower_front_block.csv"
)
add_stage_csv_gen(
    "${RESOURCE_DIR}/stage/map/tower_front/map_tower_front.tmx" 
    "${STAGE_MAP_TOWER_FONT_CSVS}"
    "${RESOURCE_DIR}/stage/map/tower_front/csv"
    --tile-or128 block
    --border block
)
set(STAGE_MAP_TOWER_FRONT_BG_CSVS
    "${RESOURCE_DIR}/stage/map/tower_front/csv/map_tower_front_bg_back.csv"
)
add_stage_csv_gen(
    "${RESOURCE_DIR}/stage/map/tower_front/map_tower_front_bg.tmx" 
    "${STAGE_MAP_TOWER_FRONT_BG_CSVS}"
    "${RESOURCE_DIR}/stage/map/tower_front/csv"
)
set(STAGE_MAP_CORRIDOR_CSVS
    "${RESOURCE_DIR}/stage/map/corridor/csv/map_corridor_block.csv"
    "${RESOURCE_DIR}/stage/map/corridor/csv/map_corridor_back.csv"
)
add_stage_csv_gen(
    "${RESOURCE_DIR}/stage/map/corridor/map_corridor.tmx"
    "${STAGE_MAP_CORRIDOR_CSVS}"
    "${RESOURCE_DIR}/stage/map/corridor/csv"
    --tile-or128 block
)
set(STAGE_MAP_STAGE_1_1_CSVS
    "${RESOURCE_DIR}/stage/map/stage_1_1/csv/map_stage_1_1_block.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_1/csv/map_stage_1_1_ladder.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_1/csv/map_stage_1_1_water.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_1/csv/map_stage_1_1_needle.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_1/csv/map_stage_1_1_one_way_block.csv"
)
add_stage_csv_gen(
    "${RESOURCE_DIR}/stage/map/stage_1_1/map_stage_1_1.tmx"
    "${STAGE_MAP_STAGE_1_1_CSVS}"
    "${RESOURCE_DIR}/stage/map/stage_1_1/csv"
    --tile-or128 block one_way_block ladder needle
)
set(STAGE_MAP_STAGE_1_2_CSVS
    "${RESOURCE_DIR}/stage/map/stage_1_2/csv/map_stage_1_2_block.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_2/csv/map_stage_1_2_ladder.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_2/csv/map_stage_1_2_water.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_2/csv/map_stage_1_2_needle.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_2/csv/map_stage_1_2_one_way_block.csv"
)
add_stage_csv_gen(
    "${RESOURCE_DIR}/stage/map/stage_1_2/map_stage_1_2.tmx"
    "${STAGE_MAP_STAGE_1_2_CSVS}"
    "${RESOURCE_DIR}/stage/map/stage_1_2/csv"
    --tile-or128 block one_way_block ladder needle
)
set(STAGE_MAP_STAGE_1_3_CSVS
    "${RESOURCE_DIR}/stage/map/stage_1_3/csv/map_stage_1_3_block.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_3/csv/map_stage_1_3_ladder.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_3/csv/map_stage_1_3_water.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_3/csv/map_stage_1_3_needle.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_3/csv/map_stage_1_3_one_way_block.csv"
)
add_stage_csv_gen(
    "${RESOURCE_DIR}/stage/map/stage_1_3/map_stage_1_3.tmx"
    "${STAGE_MAP_STAGE_1_3_CSVS}"
    "${RESOURCE_DIR}/stage/map/stage_1_3/csv"
    --tile-or128 block one_way_block ladder needle
)
set(STAGE_MAP_STAGE_1_4_CSVS
    "${RESOURCE_DIR}/stage/map/stage_1_4/csv/map_stage_1_4_block.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_4/csv/map_stage_1_4_ladder.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_4/csv/map_stage_1_4_water.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_4/csv/map_stage_1_4_needle.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_4/csv/map_stage_1_4_one_way_block.csv"
)
add_stage_csv_gen(
    "${RESOURCE_DIR}/stage/map/stage_1_4/map_stage_1_4.tmx"
    "${STAGE_MAP_STAGE_1_4_CSVS}"
    "${RESOURCE_DIR}/stage/map/stage_1_4/csv"
    --tile-or128 block one_way_block ladder needle
)
set(STAGE_MAP_STAGE_1_5_CSVS
    "${RESOURCE_DIR}/stage/map/stage_1_5/csv/map_stage_1_5_block.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_5/csv/map_stage_1_5_ladder.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_5/csv/map_stage_1_5_water.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_5/csv/map_stage_1_5_needle.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_5/csv/map_stage_1_5_one_way_block.csv"
)
add_stage_csv_gen(
    "${RESOURCE_DIR}/stage/map/stage_1_5/map_stage_1_5.tmx"
    "${STAGE_MAP_STAGE_1_5_CSVS}"
    "${RESOURCE_DIR}/stage/map/stage_1_5/csv"
    --tile-or128 block one_way_block ladder needle
)
set(STAGE_MAP_STAGE_1_6_CSVS
    "${RESOURCE_DIR}/stage/map/stage_1_6/csv/map_stage_1_6_block.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_6/csv/map_stage_1_6_ladder.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_6/csv/map_stage_1_6_water.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_6/csv/map_stage_1_6_needle.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_6/csv/map_stage_1_6_one_way_block.csv"
)
add_stage_csv_gen(
    "${RESOURCE_DIR}/stage/map/stage_1_6/map_stage_1_6.tmx"
    "${STAGE_MAP_STAGE_1_6_CSVS}"
    "${RESOURCE_DIR}/stage/map/stage_1_6/csv"
    --tile-or128 block one_way_block ladder needle
)
set(STAGE_MAP_STAGE_1_7_CSVS
    "${RESOURCE_DIR}/stage/map/stage_1_7/csv/map_stage_1_7_block.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_7/csv/map_stage_1_7_ladder.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_7/csv/map_stage_1_7_water.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_7/csv/map_stage_1_7_needle.csv"
    "${RESOURCE_DIR}/stage/map/stage_1_7/csv/map_stage_1_7_one_way_block.csv"
)
add_stage_csv_gen(
    "${RESOURCE_DIR}/stage/map/stage_1_7/map_stage_1_7.tmx"
    "${STAGE_MAP_STAGE_1_7_CSVS}"
    "${RESOURCE_DIR}/stage/map/stage_1_7/csv"
    --tile-or128 block one_way_block ladder needle
)
set(STAGE_MAP_STAGE_2_1_CSVS
    "${RESOURCE_DIR}/stage/map/stage_2_1/csv/map_stage_2_1_block.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_1/csv/map_stage_2_1_ladder.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_1/csv/map_stage_2_1_water.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_1/csv/map_stage_2_1_needle.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_1/csv/map_stage_2_1_one_way_block.csv"
)
add_stage_csv_gen(
    "${RESOURCE_DIR}/stage/map/stage_2_1/map_stage_2_1.tmx"
    "${STAGE_MAP_STAGE_2_1_CSVS}"
    "${RESOURCE_DIR}/stage/map/stage_2_1/csv"
    --tile-or128 block one_way_block ladder needle water
)
set(STAGE_MAP_STAGE_2_2_CSVS
    "${RESOURCE_DIR}/stage/map/stage_2_2/csv/map_stage_2_2_block.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_2/csv/map_stage_2_2_ladder.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_2/csv/map_stage_2_2_water.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_2/csv/map_stage_2_2_needle.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_2/csv/map_stage_2_2_one_way_block.csv"
)
add_stage_csv_gen(
    "${RESOURCE_DIR}/stage/map/stage_2_2/map_stage_2_2.tmx"
    "${STAGE_MAP_STAGE_2_2_CSVS}"
    "${RESOURCE_DIR}/stage/map/stage_2_2/csv"
    --tile-or128 block one_way_block ladder needle water
)
set(STAGE_MAP_STAGE_2_3_CSVS
    "${RESOURCE_DIR}/stage/map/stage_2_3/csv/map_stage_2_3_block.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_3/csv/map_stage_2_3_ladder.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_3/csv/map_stage_2_3_water.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_3/csv/map_stage_2_3_needle.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_3/csv/map_stage_2_3_one_way_block.csv"
)
add_stage_csv_gen(
    "${RESOURCE_DIR}/stage/map/stage_2_3/map_stage_2_3.tmx"
    "${STAGE_MAP_STAGE_2_3_CSVS}"
    "${RESOURCE_DIR}/stage/map/stage_2_3/csv"
    --tile-or128 block one_way_block ladder needle water
)
set(STAGE_MAP_STAGE_2_4_CSVS
    "${RESOURCE_DIR}/stage/map/stage_2_4/csv/map_stage_2_4_block.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_4/csv/map_stage_2_4_ladder.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_4/csv/map_stage_2_4_water.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_4/csv/map_stage_2_4_needle.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_4/csv/map_stage_2_4_one_way_block.csv"
)
add_stage_csv_gen(
    "${RESOURCE_DIR}/stage/map/stage_2_4/map_stage_2_4.tmx"
    "${STAGE_MAP_STAGE_2_4_CSVS}"
    "${RESOURCE_DIR}/stage/map/stage_2_4/csv"
    --tile-or128 block one_way_block ladder needle water
)
set(STAGE_MAP_STAGE_2_5_CSVS
    "${RESOURCE_DIR}/stage/map/stage_2_5/csv/map_stage_2_5_block.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_5/csv/map_stage_2_5_ladder.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_5/csv/map_stage_2_5_water.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_5/csv/map_stage_2_5_needle.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_5/csv/map_stage_2_5_one_way_block.csv"
)
add_stage_csv_gen(
    "${RESOURCE_DIR}/stage/map/stage_2_5/map_stage_2_5.tmx"
    "${STAGE_MAP_STAGE_2_5_CSVS}"
    "${RESOURCE_DIR}/stage/map/stage_2_5/csv"
    --tile-or128 block one_way_block ladder needle water
)
set(STAGE_MAP_STAGE_2_6_CSVS
    "${RESOURCE_DIR}/stage/map/stage_2_6/csv/map_stage_2_6_block.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_6/csv/map_stage_2_6_ladder.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_6/csv/map_stage_2_6_water.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_6/csv/map_stage_2_6_needle.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_6/csv/map_stage_2_6_one_way_block.csv"
)
add_stage_csv_gen(
    "${RESOURCE_DIR}/stage/map/stage_2_6/map_stage_2_6.tmx"
    "${STAGE_MAP_STAGE_2_6_CSVS}"
    "${RESOURCE_DIR}/stage/map/stage_2_6/csv"
    --tile-or128 block one_way_block ladder needle water
)
set(STAGE_MAP_STAGE_2_7_CSVS
    "${RESOURCE_DIR}/stage/map/stage_2_7/csv/map_stage_2_7_block.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_7/csv/map_stage_2_7_ladder.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_7/csv/map_stage_2_7_water.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_7/csv/map_stage_2_7_needle.csv"
    "${RESOURCE_DIR}/stage/map/stage_2_7/csv/map_stage_2_7_one_way_block.csv"
)
add_stage_csv_gen(
    "${RESOURCE_DIR}/stage/map/stage_2_7/map_stage_2_7.tmx"
    "${STAGE_MAP_STAGE_2_7_CSVS}"
    "${RESOURCE_DIR}/stage/map/stage_2_7/csv"
    --tile-or128 block one_way_block ladder needle water
)

set(STAGE_MAP_SHOP_CSVS
    "${RESOURCE_DIR}/stage/map/shop/csv/map_shop_back_0.csv"
    "${RESOURCE_DIR}/stage/map/shop/csv/map_shop_back_1.csv"
    "${RESOURCE_DIR}/stage/map/shop/csv/map_shop_block.csv"
)
add_stage_csv_gen(
    "${RESOURCE_DIR}/stage/map/shop/map_shop.tmx"
    "${STAGE_MAP_SHOP_CSVS}"
    "${RESOURCE_DIR}/stage/map/shop/csv"
    --border block
)

set(STAGE_MAP_CSVS
    ${STAGE_MAP_TOWER_FONT_CSVS}
    ${STAGE_MAP_TOWER_FRONT_BG_CSVS}
    ${STAGE_MAP_CORRIDOR_CSVS}
    ${STAGE_MAP_STAGE_1_1_CSVS}
    ${STAGE_MAP_STAGE_1_2_CSVS}
    ${STAGE_MAP_STAGE_1_3_CSVS}
    ${STAGE_MAP_STAGE_1_4_CSVS}
    ${STAGE_MAP_STAGE_1_5_CSVS}
    ${STAGE_MAP_STAGE_1_6_CSVS}
    ${STAGE_MAP_STAGE_1_7_CSVS}
    ${STAGE_MAP_STAGE_2_1_CSVS}
    ${STAGE_MAP_STAGE_2_2_CSVS}
    ${STAGE_MAP_STAGE_2_3_CSVS}
    ${STAGE_MAP_STAGE_2_4_CSVS}
    ${STAGE_MAP_STAGE_2_5_CSVS}
    ${STAGE_MAP_STAGE_2_6_CSVS}
    ${STAGE_MAP_STAGE_2_7_CSVS}
    ${STAGE_MAP_SHOP_CSVS}
)

foreach(CSV_FILE ${STAGE_MAP_CSVS})
    add_map_gen("${CSV_FILE}" "${GEN_DIR}/map" --compression runlength)
endforeach()

add_map_gen("${RESOURCE_DIR}/menu/map/item_name_plate.csv" "${GEN_DIR}/map" --compression runlength)

add_tileset_gen("${RESOURCE_DIR}/stage/tileset/tileset_map_elements.bmp" "${GEN_DIR}/tileset" --count 100)
add_tileset_gen("${RESOURCE_DIR}/stage/tileset/tileset_shop_elements.bmp" "${GEN_DIR}/tileset" --count 100)
add_tileset_gen("${RESOURCE_DIR}/tileset/tileset_items.bmp" "${GEN_DIR}/tileset" --count 16 --width 8 --height 8)
add_tileset_gen("${RESOURCE_DIR}/tileset/tileset_block.bmp" "${GEN_DIR}/tileset" --count 1)
add_tileset_gen("${RESOURCE_DIR}/tileset/tileset_static_objects.bmp" "${GEN_DIR}/tileset" --count 10)
add_tileset_gen("${RESOURCE_DIR}/tileset/tileset_static_objects_16x24.bmp" "${GEN_DIR}/tileset" --count 3 --width 16 --height 24)
add_tileset_gen("${RESOURCE_DIR}/tileset/tileset_static_objects_16x48.bmp" "${GEN_DIR}/tileset" --count 2 --width 16 --height 48)
add_tileset_gen("${RESOURCE_DIR}/tileset/tileset_hen.bmp" "${GEN_DIR}/tileset" --width 6 --height 10)
add_tileset_gen("${RESOURCE_DIR}/tileset/tileset_chick.bmp" "${GEN_DIR}/tileset" --width 6 --height 10)
add_tileset_gen("${RESOURCE_DIR}/tileset/tileset_florist.bmp" "${GEN_DIR}/tileset" --width 16 --height 24)
add_tileset_gen("${RESOURCE_DIR}/tileset/tileset_light_rays.bmp" "${GEN_DIR}/tileset" --count 3)
add_tileset_gen("${RESOURCE_DIR}/menu/tileset/tileset_menu_item.bmp" "${GEN_DIR}/tileset")
add_tileset_gen("${RESOURCE_DIR}/menu/tileset/tileset_menu_weapon.bmp" "${GEN_DIR}/tileset")
add_tileset_gen("${RESOURCE_DIR}/menu/tileset/tileset_menu_cursor.bmp" "${GEN_DIR}/tileset")
add_tileset_gen("${RESOURCE_DIR}/menu/tileset/tileset_menu_display.bmp" "${GEN_DIR}/tileset")

add_talkscript_gen("${RESOURCE_DIR}/talkscript/jp/talkscript_1.yml" "${GEN_DIR}/talkscript")

add_btree_gen("${RESOURCE_DIR}/btree/test_btree.yaml" "${GEN_DIR}/btree")
add_btree_gen("${RESOURCE_DIR}/btree/btree_chase.yml" "${GEN_DIR}/btree")

add_item_descriptors_gen("${RESOURCE_DIR}/item_descriptors/jp/item_descriptors_item.yml" "${GEN_DIR}/item_descriptors")
add_item_descriptors_gen("${RESOURCE_DIR}/item_descriptors/jp/item_descriptors_weapon.yml" "${GEN_DIR}/item_descriptors")

set(K8X12_SUBSET "${RESOURCE_DIR}/font/k8x12_subset.txt")
add_font_subset_gen("${K8X12_SUBSET}"
    "${RESOURCE_DIR}/talkscript/jp/talkscript_1.yml"
    "${RESOURCE_DIR}/item_descriptors/jp/item_descriptors_item.yml"
    "${RESOURCE_DIR}/item_descriptors/jp/item_descriptors_weapon.yml"
)
add_font_gen("${ASSETS_PATH}/fonts/k8x12/k8x12.bdf" "${K8X12_SUBSET}" "${GEN_DIR}/font")
add_font_gen("${ASSETS_PATH}/fonts/misaki/misaki_gothic.bdf" "${RESOURCE_DIR}/font/misaki_subset.txt" "${GEN_DIR}/font")

add_library(${PROJECT_NAME} STATIC
    ${RESOURCE_SOURCES}
)

anim_cmake_list_gen(
    "${RESOURCE_DIR}/anim/player/anim_player.yml" 
    "${RESOURCE_DIR}/tileset/tileset_player.bmp"
    "${GEN_DIR}/anim/player"
    --width 16 --height 16
)
anim_cmake_list_gen(
    "${RESOURCE_DIR}/anim/attack/anim_attack.yml" 
    "${RESOURCE_DIR}/tileset/tileset_attack.bmp"
    "${GEN_DIR}/anim/attack"
    --width 16 --height 16
)
anim_cmake_list_gen(
    "${RESOURCE_DIR}/anim/enemy/skyfish/anim_skyfish.yml" 
    "${RESOURCE_DIR}/tileset/tileset_skyfish.bmp"
    "${GEN_DIR}/anim/enemy/skyfish"
    --width 16 --height 16
)
anim_cmake_list_gen(
    "${RESOURCE_DIR}/anim/enemy/walker/anim_walker.yml" 
    "${RESOURCE_DIR}/tileset/tileset_walker.bmp"
    "${GEN_DIR}/anim/enemy/walker"
    --width 16 --height 16
)
anim_cmake_list_gen(
    "${RESOURCE_DIR}/anim/enemy/floaty_fish/anim_floaty_fish.yml" 
    "${RESOURCE_DIR}/tileset/tileset_floaty_fish.bmp"
    "${GEN_DIR}/anim/enemy/floaty_fish"
    --width 16 --height 16
)
anim_cmake_list_gen(
    "${RESOURCE_DIR}/anim/prop/portal/anim_portal.yml" 
    "${RESOURCE_DIR}/tileset/tileset_static_objects_16x24.bmp"
    "${GEN_DIR}/anim/prop/portal"
    --width 16 --height 24
)
anim_cmake_list_gen(
    "${RESOURCE_DIR}/anim/enemy/dancer/anim_boss_dancer.yml"
    "${RESOURCE_DIR}/tileset/tileset_boss_dancer.bmp"
    "${GEN_DIR}/anim/enemy/dancer"
    --width 20 --height 32 
)
add_subdirectory(mml)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    resources_config
    anim_player
    anim_attack
    anim_skyfish
    anim_walker
    anim_floaty_fish
    anim_boss_dancer
    anim_portal
    mml
)

