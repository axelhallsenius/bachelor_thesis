cmake_minimum_required(VERSION 3.16)
project(vessel_tracker)

# Set the output directory for built objects (for runtime and libraries)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Include SDL3 as a subdirectory, assuming it's inside 'vendored/SDL'
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# Build SDL3_gfx from the 'SDL3_gfx' directory
add_subdirectory(SDL3_gfx EXCLUDE_FROM_ALL)

add_executable(${PROJECT_NAME} vessel_tracker.cpp)

# Link SDL3 and SDL3_gfx to your executable
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 SDL3_gfx_Shared)

add_subdirectory(gui EXCLUDE_FROM_ALL)

# If SDL3_gfx is built as a shared library, make sure your executable knows where to find it at runtime
set_target_properties(${PROJECT_NAME} PROPERTIES
    INSTALL_RPATH "${CMAKE_BINARY_DIR}/SDL3_gfx/build"
)

# If you want to build the executable and all dependencies into the same build directory, enable this:
# set(CMAKE_BUILD_TYPE Debug)

