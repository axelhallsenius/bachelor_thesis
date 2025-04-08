cmake_minimum_required(VERSION 3.16)
project(shapes)

# Source files
set(SHAPES_SRC
  shapes.c
  #grid.c
)
set(SHAPES_H 
  shapes.h
  #grid.h
)

# Create a static library target for shapes
add_library(shapes STATIC ${SHAPES_SRC} ${SHAPES_H})

# Add include directory if needed
target_include_directories(shapes 
PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/gui
)

# Link SDL3 and SDL3_gfx (assuming SDL3_gfx defines a target named SDL3_gfx_Shared)
target_link_libraries(shapes
  PUBLIC
    SDL3::SDL3
    SDL3_gfx_Shared
    ImGui
)
