#!/bin/sh


BUILD_DIR=${BUILD_DIR:-build}
MGC_PATH=${MGC_PATH:-../../../../../}
MGC_ABS_PATH=$(realpath ${MGC_PATH})

cmake -S . -B "$BUILD_DIR" -DEXEC_NAME="psg-mml-player-core" \
                           -DMGC_USE_RP2040=OFF \
                           -DMGC_ENABLE_DISPLAY=OFF \
                           -DMGC_ENABLE_INPUT=OFF \
                           -DMGC_ENABLE_TIMER=OFF \
                           -DMGC_ABS_PATH="$MGC_ABS_PATH"

cmake --build "$BUILD_DIR" --parallel "$JOBS"


