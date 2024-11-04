
file(GLOB FILES *.c *.h)
add_library(light ${FILES})

target_link_libraries(light PUBLIC 
    pglm
)

target_include_directories(light PUBLIC .)
