add_library(terminal STATIC 
    terminal.c
    status_bar.c
)

target_include_directories(buffer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
