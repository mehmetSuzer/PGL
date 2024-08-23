
file(GLOB_RECURSE FILES *.c *.h)
add_library(pglm ${FILES})
target_include_directories(pglm PUBLIC .)
