
file(GLOB FILES *.c *.h)
add_library(transform ${FILES})

target_link_libraries(transform PUBLIC 
    pglm
)

target_include_directories(transform PUBLIC .)
