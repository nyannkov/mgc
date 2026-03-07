cmake_minimum_required(VERSION 3.14)

project(resources CXX)

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

set(STAGE_MAP_CSVS
    ${STAGE_MAP_TOWER_FONT_CSVS}
    ${STAGE_MAP_TOWER_FRONT_BG_CSVS}
    ${STAGE_MAP_CORRIDOR_CSVS}
    ${STAGE_MAP_STAGE_1_1_CSVS}
    ${STAGE_MAP_STAGE_1_2_CSVS}
    ${STAGE_MAP_STAGE_1_3_CSVS}
)

foreach(CSV_FILE ${STAGE_MAP_CSVS})
    add_map_gen("${CSV_FILE}" "${GEN_DIR}/map" --compression runlength)
endforeach()

add_map_gen("${RESOURCE_DIR}/menu/map/item_name_plate.csv" "${GEN_DIR}/map" --compression runlength)

add_tileset_gen("${RESOURCE_DIR}/stage/tileset/tileset_map_elements.bmp" "${GEN_DIR}/tileset" --count 100)
add_tileset_gen("${RESOURCE_DIR}/tileset/tileset_items.bmp" "${GEN_DIR}/tileset" --count 16 --width 8 --height 8)
add_tileset_gen("${RESOURCE_DIR}/tileset/tileset_block.bmp" "${GEN_DIR}/tileset" --count 1)
add_tileset_gen("${RESOURCE_DIR}/tileset/tileset_static_objects.bmp" "${GEN_DIR}/tileset" --count 7)
add_tileset_gen("${RESOURCE_DIR}/tileset/tileset_static_objects_16x48.bmp" "${GEN_DIR}/tileset" --count 2 --width 16 --height 48)
add_tileset_gen("${RESOURCE_DIR}/tileset/tileset_hen.bmp" "${GEN_DIR}/tileset" --width 6 --height 10)
add_tileset_gen("${RESOURCE_DIR}/tileset/tileset_chick.bmp" "${GEN_DIR}/tileset" --width 6 --height 10)
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
add_kanji_subset_gen("${K8X12_SUBSET}"
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

add_subdirectory(mml)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    resources_config
    anim_player
    anim_attack
    anim_skyfish
    mml
)

