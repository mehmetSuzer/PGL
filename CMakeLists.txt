
cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Release)
set(PICO_PLATFORM rp2040)
set(PICO_COMPILER pico_arm_cortex_m0plus_gcc)
set(PICO_GCC_TRIPLE arm-none-eabi)
set(PICO_BOARD pico)

include(pico_sdk_import.cmake)

project(pico-rast C CXX ASM)

pico_sdk_init()

add_subdirectory(src/device)
add_subdirectory(src/pglm)
add_subdirectory(src/transform)
add_subdirectory(src/light)
add_subdirectory(src/pgl)
add_subdirectory(src/camera)
add_subdirectory(src/texture)

add_executable(${PROJECT_NAME} src/main.c)

# enable/disable usb-uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    device
    pglm
    transform
    light
    pgl
    camera
    texture

    pico_multicore
)
