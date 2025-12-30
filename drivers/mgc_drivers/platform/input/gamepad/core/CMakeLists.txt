set(GAMEPAD_SRC
    "gamepad.c"
)

add_library(mgc_input_gamepad STATIC ${GAMEPAD_SRC})

target_include_directories(mgc_input_gamepad PUBLIC
    ${MGC_SRC_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp
)

