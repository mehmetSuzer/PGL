
add_library(vertex INTERFACE)

target_link_libraries(vertex INTERFACE 
    pglm
)

target_include_directories(vertex INTERFACE .)
