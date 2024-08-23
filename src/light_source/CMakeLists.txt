
add_library(light_source INTERFACE)

target_link_libraries(light_source INTERFACE 
    pglm
)

target_include_directories(light_source INTERFACE .)
