#!/bin/sh

BTREE_GEN="../../tools/btree_gen/btree_gen.py"
python3 ${BTREE_GEN}     ../common/assets/btree/test_btree.yaml --dir ./resources/generated/btree

BUILD_DIR=${BUILD_DIR:-build}
JOBS=${JOBS:-$(nproc)}

cmake -S . -B "$BUILD_DIR" -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
                           -DMGC_ENABLE_DRIVERS=OFF \
                           -DMGC_USE_RP2040=OFF \
                           -DMGC_PIXELBUF_ORDER=1

cmake --build "$BUILD_DIR" --parallel "$JOBS"
