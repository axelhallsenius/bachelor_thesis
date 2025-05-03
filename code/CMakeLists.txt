cmake_minimum_required(VERSION 3.16)
project(vessel_tracker LANGUAGES C CXX)

# Options:

option(BUILD_TESTS "Build tests" ON)  # Control test compilation

# Set the output directory for built objects (for runtime and libraries)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(SDL3IMAGE_BUILD_SHARED TRUE)
set(SDL3IMAGE_BUILD_STATIC TRUE)

if(BUILD_TESTS)
  include(FetchContent)
  # set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  # set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
  )
  FetchContent_MakeAvailable(googletest)
endif()

# target_link_libraries(jk PRIVATE gtest_main)
add_subdirectory(deps/ImGui EXCLUDE_FROM_ALL)
add_subdirectory(deps/SDL EXCLUDE_FROM_ALL)
add_subdirectory(deps/SDL3_gfx EXCLUDE_FROM_ALL)
add_subdirectory(deps/SDL_image EXCLUDE_FROM_ALL)
add_subdirectory(src)
if(BUILD_TESTS)
  add_subdirectory(tests)
endif()
# add_subdirectory(deps/SDL_image EXCLUDE_FROM_ALL)
# # Build SDL3_gfx from the 'SDL3_gfx' directory
# add_subdirectory(deps/SDL3_gfx)
# add_subdirectory(deps/ImGui)
# add_subdirectory(shapes)
# add_subdirectory(translation)
# add_subdirectory(test)

add_executable(${PROJECT_NAME} src/cpp/vessel_tracker.cpp)


# Copying assets into build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/maps"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/maps"
)
target_include_directories(${PROJECT_NAME} PRIVATE src/c)

# Link SDL3 and SDL3_gfx to your executable
target_link_libraries(${PROJECT_NAME} 
PRIVATE 
  c_code
  SDL3::SDL3 
  SDL3_image::SDL3_image
  SDL3_gfx_Shared
  ImGui
  gtest_main
  # shapes
  # translation
  # test
)


# get_property(SDLI_TARGETS DIRECTORY PROPERTY BUILDSYSTEM_TARGETS)
# message(STATUS "Available SDL_image targets: ${SDLI_TARGETS}")

# If SDL3_gfx is built as a shared library, make sure your executable knows where to find it at runtime
set_target_properties(${PROJECT_NAME} PROPERTIES
    INSTALL_RPATH "${CMAKE_BINARY_DIR}/SDL3_gfx/build"
)

# If you want to build the executable and all dependencies into the same build directory, enable this:
# set(CMAKE_BUILD_TYPE Debug)

