set(test_name indexer.test)

include(GoogleTest)

add_executable(${test_name})

include(CompileOptions)
set_compile_options(${test_name})

target_sources(
  ${test_name}
  PRIVATE
  indexer.test.cpp
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
  picosha2
)

gtest_discover_tests(${test_name})
