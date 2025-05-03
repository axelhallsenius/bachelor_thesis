# collect all .c files under src/c
file(GLOB C_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/c/*.c"
)

add_library(c_code STATIC ${C_SOURCES})

target_include_directories(c_code PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/c
)

target_include_directories(c_code PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/c)
# add_executable(vessel_tracker
#   cpp/vessel_tracker.cpp
# )

