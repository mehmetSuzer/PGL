
file(GLOB FILES *.c *.h)
add_library(pgl ${FILES})

target_link_libraries(pgl PUBLIC 
    device
    transform
    texture
)

target_include_directories(pgl PUBLIC .)
