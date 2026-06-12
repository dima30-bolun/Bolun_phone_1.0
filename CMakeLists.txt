cmake_minimum_required(VERSION 3.22)

project(BolunPhone
    VERSION 1.0.0
    DESCRIPTION "Bolun Phone 1.0 Beta classical mobile OS foundation"
    LANGUAGES C CXX ASM
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BOLUN_BUILD_TESTS "Build host contract tests" ON)

add_library(bolun_core STATIC
    boot/src/boot_config.c
    boot/src/boot_info.c
    hal/src/device_compatibility.c
    hal/src/lumia_device.c
    drivers/src/driver_abstraction.c
    drivers/display/msm_display_driver.c
    drivers/touch/lumia_touch_driver.c
    drivers/audio/audio_driver_manifest.c
    drivers/camera/camera_driver_manifest.c
    drivers/bluetooth/bluetooth_driver_manifest.c
    drivers/wifi/wifi_driver_manifest.c
    drivers/gps/gps_driver_manifest.c
    drivers/sensors/lumia_sensor_driver.c
    drivers/nfc/nfc_driver_manifest.c
    drivers/usb/usb_driver_manifest.c
    drivers/cellular/cellular_driver_manifest.c
    kernel/src/handle_manager.c
    kernel/src/ipc.c
    kernel/src/memory.c
    kernel/src/scheduler.c
    kernel/src/security.c
    graphics/src/tile_engine.cpp
    services/src/service_registry.cpp
)

target_include_directories(bolun_core PUBLIC
    boot/include
    hal/include
    drivers/include
    kernel/include
    graphics/include
    services/include
    runtime/include
    shell/include
    mainos/include
    recovery/include
    updateos/include
    sdk/native
)

target_compile_options(bolun_core PRIVATE
    $<$<COMPILE_LANG_AND_ID:C,GNU,Clang>:-Wall -Wextra -Wpedantic -Werror>
    $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang>:-Wall -Wextra -Wpedantic -Werror>
)

if(BOLUN_BUILD_TESTS)
    enable_testing()
    add_executable(bolun_core_contract_tests tests/core_contract_tests.c)
    target_link_libraries(bolun_core_contract_tests PRIVATE bolun_core)
    target_compile_options(bolun_core_contract_tests PRIVATE
        $<$<COMPILE_LANG_AND_ID:C,GNU,Clang>:-Wall -Wextra -Wpedantic -Werror>
    )
    add_test(NAME bolun_core_contract_tests COMMAND bolun_core_contract_tests)

    add_executable(bolun_cpp_contract_tests tests/cpp_contract_tests.cpp)
    target_link_libraries(bolun_cpp_contract_tests PRIVATE bolun_core)
    target_compile_options(bolun_cpp_contract_tests PRIVATE
        $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang>:-Wall -Wextra -Wpedantic -Werror>
    )
    add_test(NAME bolun_cpp_contract_tests COMMAND bolun_cpp_contract_tests)
endif()
