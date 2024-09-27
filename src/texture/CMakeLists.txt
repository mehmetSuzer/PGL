
file(GLOB FILES *.c *.h)
add_library(texture ${FILES})

target_link_libraries(texture PUBLIC 
    pglm
)

target_include_directories(texture PUBLIC .)
