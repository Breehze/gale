add_library(buffer STATIC 
    utils.c 
    translations.c
    buffer_pos.c
    buffer_mods.c
)

target_include_directories(buffer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
