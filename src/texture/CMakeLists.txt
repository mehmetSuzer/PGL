
file(GLOB FILES *.c *.h)
add_library(texture ${FILES})

target_include_directories(texture PUBLIC .)
