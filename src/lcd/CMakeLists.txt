
file(GLOB FILES *.c *.h)
add_library(lcd ${FILES})

target_link_libraries(lcd PUBLIC 
    config 
)

target_include_directories(lcd PUBLIC .)
