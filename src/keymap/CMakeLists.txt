add_library(keymap STATIC 
    impl.c
    fsm.c
    fsm_state.c
)

target_include_directories(keymap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(keymap PRIVATE buffer) 
