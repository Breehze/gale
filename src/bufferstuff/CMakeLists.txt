add_library(buffer STATIC 
    buffer_ops.c 
    utils.c 
    translations.c
)

target_include_directories(buffer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
