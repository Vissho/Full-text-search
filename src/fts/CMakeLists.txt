set(target_name fts)

add_library(${target_name} SHARED
  parser/parser.cpp
  ../../include/fts/parser.hpp
  indexer/indexer.cpp
  ../../include/fts/indexer.hpp)

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

add_subdirectory(parser)
add_subdirectory(indexer)
