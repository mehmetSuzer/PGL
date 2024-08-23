
add_library(triangle INTERFACE)

target_link_libraries(triangle INTERFACE 
    pglm
)

target_include_directories(triangle INTERFACE .)
