
file(GLOB FILES *.c *.h)
add_library(mesh ${FILES})

target_link_libraries(mesh PUBLIC 
    vertex
    camera
    rasterizer
)

target_include_directories(mesh PUBLIC .)
