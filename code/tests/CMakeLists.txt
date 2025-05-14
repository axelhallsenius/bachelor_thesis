if(BUILD_TESTS)
  add_subdirectory(unit)
endif()

if(BUILD_BENCHMARKS)
  add_subdirectory(bench)
endif()

