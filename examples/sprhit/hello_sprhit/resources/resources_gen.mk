MAKEFILE_PATH := $(dir $(abspath  $(lastword $(MAKEFILE_LIST))))

MGC_TOP_PATH ?= $(MAKEFILE_PATH)/../../../../

MAP_GEN_PATH     := $(MGC_TOP_PATH)/tools/map_gen/map_gen.py
TILESET_GEN_PATH := $(MGC_TOP_PATH)/tools/tileset_gen/tileset_gen.py
FONTSET_GEN_PATH := $(MGC_TOP_PATH)/tools/fontset_gen/fontset_gen.py
FONTS_PATH       := $(MGC_TOP_PATH)/tools/fontset_gen/fonts

RESOURCES_PATH          := $(MAKEFILE_PATH)
RESOURCES_MAP_PATH      := $(RESOURCES_PATH)/map
RESOURCES_TILESET_PATH  := $(RESOURCES_PATH)/tileset
RESOURCES_FONT_PATH     := $(RESOURCES_PATH)/font

GENERATES_PATH          := $(RESOURCES_PATH)/generates
GENERATES_MAP_PATH      := $(GENERATES_PATH)/map
GENERATES_TILESET_PATH  := $(GENERATES_PATH)/tileset
GENERATES_FONT_PATH     := $(GENERATES_PATH)/font

GENERATES_SRC := $(GENERATES_TILESET_PATH)/tileset_balls_32x32.c

.PHONY: generate
generate: $(GENERATES_SRC)

# tileset_gen
$(GENERATES_TILESET_PATH)/tileset_balls_32x32.c: $(RESOURCES_TILESET_PATH)/tileset_balls_32x32.bmp
	python3 $(TILESET_GEN_PATH) $< --dir $(GENERATES_TILESET_PATH) --count 3 --width 32 --height 32
