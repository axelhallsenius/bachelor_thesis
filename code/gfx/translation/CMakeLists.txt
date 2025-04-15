cmake_minimum_required(VERSION 3.16)
project(translation)

# Source files
set(TRANSLATION_SRC
  translation.c
)
set(TRANSLATION_H 
  translation.h
)

# Create a static library target for shapes
add_library(translation STATIC ${TRANSLATION_SRC} ${TRANSLATION_H})

# Add include directory if needed
target_include_directories(shapes 
PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/gui
)

# Link SDL3 and SDL3_gfx (assuming SDL3_gfx defines a target named SDL3_gfx_Shared)
target_link_libraries(translation
  PUBLIC
    SDL3::SDL3
    SDL3_gfx_Shared
    ImGui
)
