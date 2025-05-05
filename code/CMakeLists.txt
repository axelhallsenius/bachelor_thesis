cmake_minimum_required(VERSION 3.16)
project(vessel_tracker LANGUAGES C CXX)

# Options:

option(BUILD_TESTS "Build tests" ON)  # Control test compilation


# Set the output directory for built objects (for runtime and libraries)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(SDL3IMAGE_BUILD_SHARED TRUE)
set(SDL3IMAGE_BUILD_STATIC TRUE)



# ensure debug symbols and no optimization in Debug
set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG}   -g -O0")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")

# (optionally) enable AddressSanitizer in Debug:
set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG}   -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")

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
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets"
  COMMAND ${CMAKE_COMMAND} -E copy
    "$<TARGET_FILE:${PROJECT_NAME}>"
    "${CMAKE_SOURCE_DIR}/vessel_tracker"
)
target_include_directories(${PROJECT_NAME} PRIVATE src/c)
# target_include_directories(${PROJECT_NAME} PRIVATE deps/ImGui)

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
include(ExternalProject)

# Base directory for building and installing external projects
set(EXT_PREFIX ${CMAKE_BINARY_DIR}/external)

# Helper to install into EXT_PREFIX/install
set(EXT_INSTALL_DIR ${EXT_PREFIX}/install)

# Common CMake args for all externals
set(EXT_CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
)

### 1) SDL3
ExternalProject_Add(
    SDL3_ext
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/deps/SDL
    BINARY_DIR ${EXT_PREFIX}/SDL/build
    UPDATE_COMMAND ""            # no updates
    CONFIGURE_COMMAND cmake <SOURCE_DIR> ${EXT_CMAKE_ARGS} -DSDL_SHARED=ON -DSDL_STATIC=OFF
    BUILD_COMMAND cmake --build .
    INSTALL_COMMAND cmake --build . --target install
)

### 2) SDL3_gfx (depends on SDL3)
ExternalProject_Add(
    SDL3_gfx_ext
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/deps/SDL3_gfx
    BINARY_DIR ${EXT_PREFIX}/SDL3_gfx/build
    DEPENDS SDL3_ext
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND cmake <SOURCE_DIR> ${EXT_CMAKE_ARGS} -DCMAKE_PREFIX_PATH=${EXT_INSTALL_DIR}
    BUILD_COMMAND cmake --build .
    INSTALL_COMMAND cmake --build . --target install
)

### 3) SDL3_image (depends on SDL3)
ExternalProject_Add(
    SDL3_image_ext
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/deps/SDL_image
    BINARY_DIR ${EXT_PREFIX}/SDL_image/build
    DEPENDS SDL3_ext
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND cmake <SOURCE_DIR> ${EXT_CMAKE_ARGS} -DSDL3IMAGE_SHARED=ON -DSDL3IMAGE_STATIC=OFF -DCMAKE_PREFIX_PATH=${EXT_INSTALL_DIR}
    BUILD_COMMAND cmake --build .
    INSTALL_COMMAND cmake --build . --target install
)

### 4) ImGui (depends on SDL3)
ExternalProject_Add(
    ImGui_ext
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/deps/ImGui
    BINARY_DIR ${EXT_PREFIX}/ImGui/build
    DEPENDS SDL3_ext
    UPDATE_COMMAND ""            # disable git updates
    CONFIGURE_COMMAND cmake <SOURCE_DIR> ${EXT_CMAKE_ARGS} -DCMAKE_PREFIX_PATH=${EXT_INSTALL_DIR}
    BUILD_COMMAND cmake --build .
    INSTALL_COMMAND cmake --build . --target install
)
