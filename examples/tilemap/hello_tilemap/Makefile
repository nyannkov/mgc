MAKEFILE_PATH := $(dir $(abspath  $(lastword $(MAKEFILE_LIST))))

PROJ_NAME := $(shell basename "$(MAKEFILE_PATH)")
OUTPUT_DIR := build
ELF_FILE_PATH := $(MAKEFILE_PATH)/$(OUTPUT_DIR)/$(PROJ_NAME).elf

#BUILD_CONF := Debug
BUILD_CONF := Release
OPENOCD_PATH := $(PICO_SDK_PATH)/../openocd

#$(ELF_FILE_PATH): build

.PHONY: build
build:
	make -f resources/resources_gen.mk
	mkdir -p $(OUTPUT_DIR)
	cmake -DCMAKE_BUILD_TYPE=$(BUILD_CONF) -DCMAKE_PROJECT_NAME=$(PROJ_NAME) -S $(MAKEFILE_PATH) -B $(MAKEFILE_PATH)/$(OUTPUT_DIR)
	make -C  $(OUTPUT_DIR)

.PHONY: clean
clean:
	rm -rf resources/generates
	rm -rf $(OUTPUT_DIR)

.PHONY: connect
connect:
	gnome-terminal -- $(OPENOCD_PATH)/src/openocd -f pico-jlink.cfg -s $(OPENOCD_PATH)/tcl

.PHONY: debug
debug: $(ELF_FILE_PATH)
	gdb-multiarch -x .gdbinit $<

