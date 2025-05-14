cmake_minimum_required(VERSION 3.16)
project(vessel_tracker LANGUAGES C CXX)

# Build options
option(BUILD_TESTS "Build tests" ON)
option(BUILD_BENCHMARKS "Build performance benchmarks" ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# RPATH so shared libs are found at runtime
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "$ORIGIN")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Debug flags
set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG} -g -O0 -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")

# Dependencies (in-tree)
add_subdirectory(deps/ImGui EXCLUDE_FROM_ALL)
add_subdirectory(deps/SDL EXCLUDE_FROM_ALL)
add_subdirectory(deps/SDL3_gfx EXCLUDE_FROM_ALL)
add_subdirectory(deps/SDL_image EXCLUDE_FROM_ALL)

# Source code
add_subdirectory(src)

# Main executable
add_executable(${PROJECT_NAME} src/cpp/vessel_tracker.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE src/c)

target_link_libraries(${PROJECT_NAME} 
  PRIVATE
    c_code
    SDL3::SDL3 
    SDL3_image::SDL3_image
    SDL3_gfx_Shared
    ImGui
)

# Copy assets directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
)

# Copy required .so libraries to runtime dir
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SDL3-shared>
    $<TARGET_FILE:SDL3_image-shared>
    $<TARGET_FILE:SDL3_gfx_Shared>
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# Optional: copy final binary to root for convenience
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
    "$<TARGET_FILE:${PROJECT_NAME}>"
    "${CMAKE_SOURCE_DIR}/vessel_tracker"
)

# Tests and benchmarks
if(BUILD_TESTS)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
  )
  FetchContent_MakeAvailable(googletest)
  add_subdirectory(tests)
endif()

if(BUILD_BENCHMARKS)
  include(FetchContent)
  FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.8.4
  )

  set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "Disable benchmark's gtest tests" FORCE)
  set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark's internal testing" FORCE)

  FetchContent_MakeAvailable(benchmark)
  add_subdirectory(tests)
endif()
