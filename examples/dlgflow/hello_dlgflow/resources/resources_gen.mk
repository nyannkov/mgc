MAKEFILE_PATH := $(dir $(abspath  $(lastword $(MAKEFILE_LIST))))

MGC_TOP_PATH ?= $(MAKEFILE_PATH)/../../../../

MAP_GEN_PATH     := $(MGC_TOP_PATH)/tools/map_gen/map_gen.py
TILESET_GEN_PATH := $(MGC_TOP_PATH)/tools/tileset_gen/tileset_gen.py
FONT_GEN_PATH    := $(MGC_TOP_PATH)/tools/font_gen/font_gen.py
FONTS_PATH       := $(MGC_TOP_PATH)/fonts

RESOURCES_PATH          := $(MAKEFILE_PATH)
RESOURCES_MAP_PATH      := $(RESOURCES_PATH)/map
RESOURCES_TILESET_PATH  := $(RESOURCES_PATH)/tileset
RESOURCES_FONT_PATH     := $(RESOURCES_PATH)/font

GENERATES_PATH          := $(RESOURCES_PATH)/generates
GENERATES_MAP_PATH      := $(GENERATES_PATH)/map
GENERATES_TILESET_PATH  := $(GENERATES_PATH)/tileset
GENERATES_FONT_PATH     := $(GENERATES_PATH)/font

GENERATES_SRC := $(GENERATES_FONT_PATH)/k8x12S.c

.PHONY: generate
generate: $(GENERATES_SRC)

# font_gen
$(GENERATES_FONT_PATH)/k8x12S.c: $(FONTS_PATH)/k8x12_bdf/k8x12S.bdf
	python3 $(FONT_GEN_PATH) $< --dir $(GENERATES_FONT_PATH) --subsettext $(RESOURCES_FONT_PATH)/subset.txt

