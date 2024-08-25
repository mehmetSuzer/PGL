
cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Release)
set(PICO_PLATFORM rp2040)
set(PICO_COMPILER pico_arm_gcc)
set(PICO_BOARD pico)

include(pico_sdk_import.cmake)

project(pico-rast C CXX ASM)

pico_sdk_init()

add_subdirectory(src/pglm)
add_subdirectory(src/color)
add_subdirectory(src/light_source)
add_subdirectory(src/camera)
add_subdirectory(src/rasterizer)
add_subdirectory(src/vertex)
add_subdirectory(src/mesh)

add_subdirectory(src/config)
add_subdirectory(src/lcd)

add_executable(${PROJECT_NAME} src/main.c)

# enable/disable usb-uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    pglm
    color
    light_source
    camera
    rasterizer
    vertex
    mesh
    
    config
    lcd

    pico_stdlib
    pico_multicore
    hardware_spi
)
