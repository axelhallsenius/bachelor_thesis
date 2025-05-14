add_executable(benchmark_suite bench_all.cpp)
target_link_libraries(benchmark_suite PRIVATE 
  c_code
  benchmark::benchmark
  SDL3::SDL3      
  SDL3_image::SDL3_image  
  SDL3_gfx_Shared 
)
target_compile_features(benchmark_suite PRIVATE cxx_std_17)

target_include_directories(unit_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/c  # Access C headers
)
