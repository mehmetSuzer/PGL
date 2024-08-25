
file(GLOB FILES *.c *.h)
add_library(rasterizer ${FILES})

target_link_libraries(rasterizer PUBLIC 
    device
    pglm
)

target_include_directories(rasterizer PUBLIC .)
