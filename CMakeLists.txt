cmake_minimum_required(VERSION 3.10)
project(editor C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

include_directories(${PROJECT_SOURCE_DIR}/include)

# Process subdirectories
add_subdirectory(src/bufferstuff)
add_subdirectory(src/terminal)

# Main executable
add_executable(gale src/main.c)

# Link the module libraries
target_link_libraries(gale buffer terminal)
