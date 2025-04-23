cmake_minimum_required(VERSION 3.16)
project(test)

# Source files
set(TEST_SRC
  test.c
)
set(TEST_H 
  test.h
)

# Create a static library target for shapes
add_library(test STATIC ${TEST_SRC} ${TEST_H})

# Add include directory if needed
target_include_directories(test 
PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/gui
  ${CMAKE_SOURCE_DIR}/shapes
)

# Link SDL3 and SDL3_gfx (assuming SDL3_gfx defines a target named SDL3_gfx_Shared)
target_link_libraries(test
  PUBLIC
    SDL3::SDL3
    SDL3_gfx_Shared
    ImGui
    shapes
)
