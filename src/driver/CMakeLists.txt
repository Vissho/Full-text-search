set(target_name indexcomm)

add_executable(${target_name})

include(CompileOptions)
set_compile_options(${target_name})

target_sources(
  ${target_name}
  PRIVATE
    indexcomm.cpp
)

target_link_libraries(
  ${target_name}
  PRIVATE
    fts
    cxxopts
    nlohmann_json
    rapidcsv
)

set(target_name1 searchcomm)

add_executable(${target_name1})

include(CompileOptions)
set_compile_options(${target_name1})

target_sources(
  ${target_name1}
  PRIVATE
    searchcomm.cpp
)

target_link_libraries(
  ${target_name1}
  PRIVATE
    fts
    cxxopts
    nlohmann_json
    replxx
)
