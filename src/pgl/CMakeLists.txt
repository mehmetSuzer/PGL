
file(GLOB FILES *.c *.h)
add_library(pgl ${FILES})

target_link_libraries(pgl PUBLIC 
    device
    transform
)

target_include_directories(pgl PUBLIC .)
