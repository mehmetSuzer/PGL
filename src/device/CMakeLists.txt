
file(GLOB FILES *.c *.h)
add_library(device ${FILES})

target_link_libraries(device PUBLIC 
    pico_stdlib 
    hardware_spi 
    hardware_pwm 
)

target_include_directories(device PUBLIC .)
