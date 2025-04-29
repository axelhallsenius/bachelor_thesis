cmake_minimum_required(VERSION 3.16)
project(vessel_tracker)

#enable ctest
# include(CTest)

# Set the output directory for built objects (for runtime and libraries)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(SDL3IMAGE_BUILD_SHARED TRUE)
set(SDL3IMAGE_BUILD_STATIC TRUE)

# Include SDL3 as a subdirectory, assuming it's inside 'vendored/SDL'
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)
# Build SDL3_gfx from the 'SDL3_gfx' directory
add_subdirectory(SDL3_gfx)
add_subdirectory(gui)
add_subdirectory(shapes)
add_subdirectory(translation)
add_subdirectory(test)

add_executable(${PROJECT_NAME} vessel_tracker.cpp)

# add_test(geotest geotest)

# Copying assets into build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets/maps"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/maps"
)

# Link SDL3 and SDL3_gfx to your executable
target_link_libraries(${PROJECT_NAME} 
PRIVATE 
  SDL3::SDL3 
  SDL3_image::SDL3_image
  SDL3_gfx_Shared
  # ImGui
  shapes
  translation
  test
)
# target_include_directories(${PROJECT_NAME} PRIVATE shapes)

get_property(SDLI_TARGETS DIRECTORY PROPERTY BUILDSYSTEM_TARGETS)
message(STATUS "Available SDL_image targets: ${SDLI_TARGETS}")

# If SDL3_gfx is built as a shared library, make sure your executable knows where to find it at runtime
set_target_properties(${PROJECT_NAME} PROPERTIES
    INSTALL_RPATH "${CMAKE_BINARY_DIR}/SDL3_gfx/build"
)

# If you want to build the executable and all dependencies into the same build directory, enable this:
# set(CMAKE_BUILD_TYPE Debug)

