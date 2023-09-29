set(target_name fts)

add_library(${target_name} SHARED
  parser.cpp
  ../../include/fts/parser.hpp)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
  ../../include/
)

target_link_libraries(
  ${target_name}
  PRIVATE
    nlohmann_json
)

set(test_name parser.test)

include(GoogleTest)

add_executable(${test_name})

include(CompileOptions)
set_compile_options(${test_name})

target_sources(
  ${test_name}
  PRIVATE
  parser.test.cpp
)

target_include_directories(
  ${test_name}
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(
  ${test_name}
  PRIVATE
  GTest::gtest_main
  fts
  nlohmann_json
)

gtest_discover_tests(${test_name})
