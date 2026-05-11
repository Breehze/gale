cmake_minimum_required(VERSION 3.10)
project(editor C)

set(CMAKE_C_STANDARD 99)

add_compile_options(-Wall -Wextra -g)

add_compile_options(-fsanitize=address -fsanitize=undefined -fsanitize=leak)
add_link_options(-fsanitize=address -fsanitize=undefined -fsanitize=leak)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(src/bufferstuff)
add_subdirectory(src/terminal)
add_subdirectory(src/keymap)

add_executable(gale src/main.c)

target_link_libraries(gale buffer terminal keymap)
