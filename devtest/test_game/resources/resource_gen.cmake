set(MGC_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../" CACHE PATH "Root directory of the mgc")
get_filename_component(MGC_ROOT_ABS "${CMAKE_CURRENT_SOURCE_DIR}/../../../" ABSOLUTE)

set(ASSETS_PATH "${MGC_ROOT_ABS}/assets" CACHE PATH "Path to raw assets")
set(TOOLS_PATH "${MGC_ROOT_ABS}/tools" CACHE PATH "Path to python tools")
set(TOOLS_EX_PATH "${TEST_GAME_DIR}/tools" CACHE PATH "Path to additional python tools")
set(RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH "Directory for resource files")
set(GEN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/generated" CACHE PATH "Directory for generated files")

set(TILESET_GEN          "${TOOLS_PATH}/tileset_gen/tileset_gen.py")
set(MAP_GEN              "${TOOLS_PATH}/map_gen/map_gen.py")
set(FONT_GEN             "${TOOLS_PATH}/font_gen/font_gen.py")
set(TALKSCRIPT_GEN       "${TOOLS_PATH}/talkscript_gen/talkscript_gen.py")
set(BTREE_GEN            "${TOOLS_PATH}/btree_gen/btree_gen.py")
set(ANIM_GEN             "${TOOLS_PATH}/anim_gen/anim_gen.py")
set(ANIM_FRAME_SPLITTER  "${TOOLS_PATH}/anim_frame_splitter/anim_frame_splitter.py")
set(STAGE_GEN            "${TOOLS_EX_PATH}/stage_gen/stage_gen.py")
set(KANJI_SUBSET_GEN     "${TOOLS_EX_PATH}/kanji_subset_gen/kanji_subset_gen.py")
set(ITEM_DESCRIPTORS_GEN "${TOOLS_EX_PATH}/item_descriptors_gen/item_descriptors_gen.py")

find_package(Python3 REQUIRED)
set(PY3 ${Python3_EXECUTABLE})

function(add_tileset_gen INPUT_BMP OUT_DIR)
    get_filename_component(BASE_NAME "${INPUT_BMP}" NAME_WE)
    set(OUT_C "${OUT_DIR}/${BASE_NAME}.c")

    add_custom_command(
        OUTPUT "${OUT_C}"

        COMMAND "${PY3}" "${TILESET_GEN}" "${INPUT_BMP}" --dir "${OUT_DIR}" ${ARGN}
        DEPENDS "${INPUT_BMP}" "${TILESET_GEN}"
        COMMENT "Generating: ${BASE_NAME}.c"
        VERBATIM
    )

    list(APPEND RESOURCE_SOURCES "${OUT_C}")
    set(RESOURCE_SOURCES "${RESOURCE_SOURCES}" PARENT_SCOPE)
endfunction()

function(add_talkscript_gen INPUT_YAML OUT_DIR)
    get_filename_component(BASE_NAME "${INPUT_YAML}" NAME_WE)
    set(OUT_C "${OUT_DIR}/${BASE_NAME}.c")

    add_custom_command(
        OUTPUT "${OUT_C}"

        COMMAND "${PY3}" "${TALKSCRIPT_GEN}" "${INPUT_YAML}" --dir "${OUT_DIR}" ${ARGN}
        DEPENDS "${INPUT_YAML}" "${TALKSCRIPT_GEN}"
        COMMENT "Generating: ${BASE_NAME}.c"
        VERBATIM
    )

    list(APPEND RESOURCE_SOURCES "${OUT_C}")
    set(RESOURCE_SOURCES "${RESOURCE_SOURCES}" PARENT_SCOPE)
endfunction()


function(add_btree_gen INPUT_YAML OUT_DIR)
    get_filename_component(BASE_NAME "${INPUT_YAML}" NAME_WE)
    set(OUT_C "${OUT_DIR}/${BASE_NAME}.c")

    add_custom_command(
        OUTPUT "${OUT_C}"

        COMMAND "${PY3}" "${BTREE_GEN}" "${INPUT_YAML}" --dir "${OUT_DIR}" ${ARGN}
        DEPENDS "${INPUT_YAML}" "${BTREE_GEN}"
        COMMENT "Generating: ${BASE_NAME}.c"
        VERBATIM
    )

    list(APPEND RESOURCE_SOURCES "${OUT_C}")
    set(RESOURCE_SOURCES "${RESOURCE_SOURCES}" PARENT_SCOPE)
endfunction()

function(add_item_descriptors_gen INPUT_YAML OUT_DIR)
    get_filename_component(BASE_NAME "${INPUT_YAML}" NAME_WE)
    set(OUT_C "${OUT_DIR}/${BASE_NAME}.c")

    add_custom_command(
        OUTPUT "${OUT_C}"

        COMMAND "${PY3}" "${ITEM_DESCRIPTORS_GEN}" "${INPUT_YAML}" --dir "${OUT_DIR}" ${ARGN}
        DEPENDS "${INPUT_YAML}" "${ITEM_DESCRIPTORS_GEN}"
        COMMENT "Generating: ${BASE_NAME}.c"
        VERBATIM
    )

    list(APPEND RESOURCE_SOURCES "${OUT_C}")
    set(RESOURCE_SOURCES "${RESOURCE_SOURCES}" PARENT_SCOPE)
endfunction()

function(add_kanji_subset_gen OUTPUT_TXT)
    set(YAML_INPUTS "")
    set(PY_ARGS "")

    foreach(YML ${ARGN})
        list(APPEND YAML_INPUTS "${YML}")
        list(APPEND PY_ARGS -s "${YML}")
    endforeach()

    add_custom_command(
        OUTPUT "${OUTPUT_TXT}"
        COMMAND "${PY3}" "${KANJI_SUBSET_GEN}" ${PY_ARGS} -d "${OUTPUT_TXT}"
        DEPENDS ${YAML_INPUTS} "${KANJI_SUBSET_GEN}"
        COMMENT "Generating kanji subset: ${OUTPUT_TXT}"
        VERBATIM
    )
endfunction()

function(add_font_gen INPUT_BDF SUBSET_TXT OUT_DIR)
    get_filename_component(BASE_NAME "${INPUT_BDF}" NAME_WE)
    set(OUT_C "${OUT_DIR}/${BASE_NAME}.c")

    add_custom_command(
        OUTPUT "${OUT_C}"
        COMMAND "${PY3}" "${FONT_GEN}" "${INPUT_BDF}" --subset "${SUBSET_TXT}" --dir "${OUT_DIR}"
        DEPENDS "${INPUT_BDF}" "${SUBSET_TXT}" "${FONT_GEN}"
        COMMENT "Generating font: ${BASE_NAME}.c"
        VERBATIM
    )

    set(RESOURCE_SOURCES ${RESOURCE_SOURCES} "${OUT_C}" PARENT_SCOPE)
endfunction()

function(add_stage_csv_gen INPUT_TMX OUTPUT_CSVS OUT_DIR)
    add_custom_command(
        OUTPUT ${OUTPUT_CSVS}
        COMMAND "${PY3}" "${STAGE_GEN}" "${INPUT_TMX}" --dir "${OUT_DIR}" ${ARGN}
        DEPENDS "${INPUT_TMX}" "${STAGE_GEN}"
        COMMENT "Splitting TMX to CSVs: ${INPUT_TMX}"
        VERBATIM
    )
endfunction()

function(add_map_gen INPUT_CSV OUT_DIR)
    get_filename_component(BASE_NAME "${INPUT_CSV}" NAME_WE)
    set(OUT_C "${OUT_DIR}/${BASE_NAME}.c")

    add_custom_command(
        OUTPUT "${OUT_C}"
        COMMAND "${PY3}" "${MAP_GEN}" "${INPUT_CSV}" --dir "${OUT_DIR}" ${ARGN}
        DEPENDS "${INPUT_CSV}" "${MAP_GEN}"
        COMMENT "Generating: ${BASE_NAME}.c"
        VERBATIM
    )

    list(APPEND RESOURCE_SOURCES "${OUT_C}")
    set(RESOURCE_SOURCES "${RESOURCE_SOURCES}" PARENT_SCOPE)
endfunction()

function(anim_cmake_list_gen INPUT_YAML INPUT_BMP OUT_DIR)

    get_filename_component(BASE_NAME "${INPUT_YAML}" NAME_WE)
    get_filename_component(YAML_DIR "${INPUT_YAML}" DIRECTORY)
    get_filename_component(YAML_ABS_DIR "${YAML_DIR}" ABSOLUTE)

    set(OUT_TXT "${OUT_DIR}/CMakeLists.txt")

    set(MUST_EXECUTE FALSE)
    if(NOT EXISTS "${OUT_TXT}")
        set(MUST_EXECUTE TRUE)
    else()
        if("${INPUT_YAML}" IS_NEWER_THAN "${OUT_TXT}" OR
           "${INPUT_BMP}" IS_NEWER_THAN "${OUT_TXT}" OR
           "${ANIM_GEN}" IS_NEWER_THAN "${OUT_TXT}")
            set(MUST_EXECUTE TRUE)
        endif()
    endif()

    if(MUST_EXECUTE)
        file(MAKE_DIRECTORY "${OUT_DIR}")
        execute_process(
            COMMAND "${PY3}" "${ANIM_FRAME_SPLITTER}" "${INPUT_BMP}" --dir "${YAML_ABS_DIR}" ${ARGN}
            RESULT_VARIABLE RET
        )
        if(NOT RET EQUAL 0)
            message(FATAL_ERROR "${INPUT_BMP}: Split failed")
        endif()
        execute_process(
            COMMAND "${PY3}" "${ANIM_GEN}" "${INPUT_YAML}" --dir "${OUT_DIR}"
            RESULT_VARIABLE RET
        )
        if(NOT RET EQUAL 0)
            message(FATAL_ERROR "${BASE_NAME}: Generation failed")
        endif()
    endif()

    add_subdirectory("${OUT_DIR}")

    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${INPUT_YAML}" "${INPUT_BMP}")
endfunction()

