
file(GLOB FILES *.c *.h)
add_library(config ${FILES})

target_link_libraries(config PUBLIC 
    pico_stdlib 
    hardware_spi 
    hardware_pwm 
)

target_include_directories(config PUBLIC .)
