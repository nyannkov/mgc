set(DIGITAL_GAMEPAD_SRC
    "digital_gamepad.c"
)

if(MGC_USE_RP2040)
    list(APPEND DIGITAL_GAMEPAD_SRC
        "ports/rp2040/digital_gamepad_rp2040.c"
    )
endif()

add_library(mgc_input_digital_gamepad STATIC ${DIGITAL_GAMEPAD_SRC})

option(MGC_USE_DEFAULT_DIGITAL_GAMEPAD "Enable and use the default digital gamepad configuration (pins and keys)" ON)
if(MGC_USE_DEFAULT_DIGITAL_GAMEPAD)
    target_compile_definitions(mgc_input_digital_gamepad PUBLIC MGC_DIGITAL_GAMEPAD_USE_DEFAULT_PIN_CONFIG)
endif()


target_include_directories(mgc_input_digital_gamepad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${MGC_SRC_DIR}
    ${MGC_DRIVERS_DIR}
)

if(MGC_ENABLE_CPP_INTERFACE)
    target_include_directories(mgc_input_digital_gamepad PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp
    )
endif()

if(MGC_USE_RP2040)
    target_link_libraries(mgc_input_digital_gamepad 
        pico_stdlib
        hardware_gpio
    )
endif()

