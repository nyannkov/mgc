#!/bin/sh
set -e  # Stop on error

SIM="${SIM:-OFF}"

if [ "$SIM" = "ON" ]; then
    USE_RP2040=OFF
    SCENE_POOL_SIZE=4096
    NOSWAP=1
    BUILD_DIR=${BUILD_DIR:-sim_build}
    EXEC_NAME=test_game
else
    USE_RP2040=ON
    SCENE_POOL_SIZE=8192
    NOSWAP=0
    BUILD_DIR=${BUILD_DIR:-build}
    EXEC_NAME=test_game.elf
    if [ -n "$PICO_SDK_PATH" ]; then
        # RP2040 toolchain (GCC for Cortex-M0+)
        TOOLCHAIN_FILE="${PICO_SDK_PATH}/cmake/preload/toolchains/pico_arm_cortex_m0plus_gcc.cmake"
    else
        echo "Error: PICO_SDK_PATH is not set and TOOLCHAIN_FILE is not provided."
        exit 1
    fi
fi


TOOLS_PATH="../../tools"
FONTS_PATH="../../assets/fonts"

rm -rf ./resources/generated/*

TILESET_GEN="${TOOLS_PATH}/tileset_gen/tileset_gen.py"
MAP_GEN="${TOOLS_PATH}/map_gen/map_gen.py"
FONT_GEN="${TOOLS_PATH}/font_gen/font_gen.py"
TALKSCRIPT_GEN="${TOOLS_PATH}/talkscript_gen/talkscript_gen.py"
BTREE_GEN="${TOOLS_PATH}/btree_gen/btree_gen.py"
ANIM_GEN="${TOOLS_PATH}/anim_gen/anim_gen.py"
ANIM_FRAME_SPLITTER="${TOOLS_PATH}/anim_frame_splitter/anim_frame_splitter.py"
STAGE_GEN="tools/stage_gen/stage_gen.py"
KANJI_SUBSET_GEN="tools/kanji_subset_gen/kanji_subset_gen.py"

#python3 ${ANIM_FRAME_SPLITTER}    ./resources/tileset/tileset_skyfish.bmp --width 16 --height 16 --dir ./resources/anim/enemy/skyfish
#python3 ${ANIM_FRAME_SPLITTER}    ./resources/tileset/tileset_player.bmp --width 16 --height 16 --dir ./resources/anim/player

python3 ${ANIM_GEN}         ./resources/anim/player/anim_player.yml  --dir ./resources/generated/anim/player
python3 ${ANIM_GEN}         ./resources/anim/enemy/skyfish/anim_skyfish.yml  --dir ./resources/generated/anim/enemy/skyfish

python3 ${TILESET_GEN}      ./resources/stage/tileset/tileset_map_elements.bmp --count 100 --dir ./resources/generated/tileset
python3 ${TILESET_GEN}      ./resources/tileset/tileset_items.bmp --count 16 --width 8 --height 8 --dir ./resources/generated/tileset
python3 ${TILESET_GEN}      ./resources/tileset/tileset_attack.bmp --count 2 --width 8 --height 16 --dir ./resources/generated/tileset
python3 ${TILESET_GEN}      ./resources/tileset/tileset_static_objects.bmp --count 7 --dir ./resources/generated/tileset
python3 ${TILESET_GEN}      ./resources/tileset/tileset_static_objects_16x48.bmp --count 2 --width 16 --height 48 --dir ./resources/generated/tileset
python3 ${TILESET_GEN}      ./resources/tileset/tileset_hen.bmp --width 6 --height 10 --dir ./resources/generated/tileset
python3 ${TILESET_GEN}      ./resources/tileset/tileset_chick.bmp --width 6 --height 10 --dir ./resources/generated/tileset
python3 ${TILESET_GEN}      ./resources/tileset/tileset_light_rays.bmp --count 3 --dir ./resources/generated/tileset


rm -rf ./resources/stage/map/tower_front/csv*
python3 ${STAGE_GEN}        ./resources/stage/map/tower_front/map_tower_front.tmx --tile-or128 block --border block --dir ./resources/stage/map/tower_front/csv/
python3 ${STAGE_GEN}        ./resources/stage/map/tower_front/map_tower_front_bg.tmx --dir ./resources/stage/map/tower_front/csv/
python3 ${MAP_GEN}          ./resources/stage/map/tower_front/csv/map_tower_front_back_tower.csv --compression runlength --dir ./resources/generated/map
python3 ${MAP_GEN}          ./resources/stage/map/tower_front/csv/map_tower_front_block.csv --compression runlength --dir ./resources/generated/map
python3 ${MAP_GEN}          ./resources/stage/map/tower_front/csv/map_tower_front_bg_back.csv --compression runlength --dir ./resources/generated/map

rm -rf ./resources/stage/map/corridor/csv*
python3 ${STAGE_GEN}        ./resources/stage/map/corridor/map_corridor.tmx --tile-or128 block --dir ./resources/stage/map/corridor/csv/
python3 ${MAP_GEN}          ./resources/stage/map/corridor/csv/map_corridor_block.csv --compression runlength --dir ./resources/generated/map
python3 ${MAP_GEN}          ./resources/stage/map/corridor/csv/map_corridor_back.csv --compression runlength --dir ./resources/generated/map

rm -rf ./resources/stage/map/stage_1_1/csv*
python3 ${STAGE_GEN}        ./resources/stage/map/stage_1_1/map_stage_1_1.tmx --tile-or128 block one_way_block ladder water needle --dir ./resources/stage/map/stage_1_1/csv
python3 ${MAP_GEN}          ./resources/stage/map/stage_1_1/csv/map_stage_1_1_block.csv --compression runlength --dir ./resources/generated/map
python3 ${MAP_GEN}          ./resources/stage/map/stage_1_1/csv/map_stage_1_1_ladder.csv --compression runlength --dir ./resources/generated/map
python3 ${MAP_GEN}          ./resources/stage/map/stage_1_1/csv/map_stage_1_1_water.csv --compression runlength --dir ./resources/generated/map
python3 ${MAP_GEN}          ./resources/stage/map/stage_1_1/csv/map_stage_1_1_needle.csv --compression runlength --dir ./resources/generated/map
python3 ${MAP_GEN}          ./resources/stage/map/stage_1_1/csv/map_stage_1_1_one_way_block.csv --compression runlength --dir ./resources/generated/map

python3 ${TALKSCRIPT_GEN}   ./resources/talkscript/jp/talkscript_1.yml --dir ./resources/generated/talkscript

python3 ${BTREE_GEN}        ./resources/btree/test_btree.yaml --dir ./resources/generated/btree
python3 ${BTREE_GEN}        ./resources/btree/btree_chase.yml --dir ./resources/generated/btree

python3 ${KANJI_SUBSET_GEN} -s ./resources/talkscript/jp/talkscript_1.yml -d ./resources/font/k8x12_subset.txt
python3 ${FONT_GEN}         ${FONTS_PATH}/k8x12/k8x12.bdf  --subset  ./resources/font/k8x12_subset.txt  --dir ./resources/generated/font
python3 ${FONT_GEN}         ${FONTS_PATH}/misaki/misaki_gothic.bdf  --subset  ./resources/font/misaki_subset.txt  --dir ./resources/generated/font


JOBS=${JOBS:-$(nproc)}

cmake -S . -B "$BUILD_DIR" -DEXEC_NAME="$EXEC_NAME" \
                           -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
                           -DMGC_USE_DEFAULT_DIGITAL_GAMEPAD=ON \
                           -DMGC_PIXELBUF_NOSWAP="$NOSWAP" \
                           -DMGC_PIXELBUF_ORDER=1 \
                           -DMGC_MAP_TILESET_INDEX_OFFSET=1 \
                           -DMGC_USE_RP2040="$USE_RP2040" \
                           -DSIM_BUILD="$SIM"

cmake --build "$BUILD_DIR" --parallel "$JOBS"


