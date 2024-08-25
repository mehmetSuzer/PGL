
file(GLOB FILES *.c *.h)
add_library(camera ${FILES})

target_link_libraries(camera PUBLIC 
    device
    pglm
)

target_include_directories(camera PUBLIC .)
