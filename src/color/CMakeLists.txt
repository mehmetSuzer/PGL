
add_library(color INTERFACE)

target_link_libraries(color INTERFACE 
    pglm
)

target_include_directories(color INTERFACE .)
