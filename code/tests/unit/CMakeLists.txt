# Unit tests (C++)
# collecting all .cpp in tests/cpp
file(GLOB CPP_TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
# building test executable from them
add_executable(unit_tests ${CPP_TEST_SOURCES})

target_link_libraries(unit_tests PRIVATE
  c_code        
  gtest_main     
  SDL3::SDL3      
  SDL3_image::SDL3_image  
  SDL3_gfx_Shared 
)


# Pure C tests (if using Unity)
# add_executable(c_tests
#   # c/test_module1.c
# )
# target_link_libraries(c_tests PRIVATE c_code)

target_include_directories(unit_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/c  # Access C headers
)

set_target_properties(unit_tests PROPERTIES
  BUILD_RPATH "$<TARGET_FILE_DIR:gtest_main>"
  # INSTALL_RPATH "$<TARGET_FILE_DIR:gtest_main>"
)

# Add test cases
add_test(NAME unit_tests COMMAND unit_tests)

# Copy GTest .so files next to unit_tests binary
add_custom_command(TARGET unit_tests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:gtest>
    $<TARGET_FILE:gtest_main>
    $<TARGET_FILE_DIR:unit_tests>
)
