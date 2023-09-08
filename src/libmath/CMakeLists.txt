set(target_name fts)

add_library(${target_name} SHARED
  sum.cpp
  ../../include/fts/sum.h)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
    ../../include/
    ../../external/cxxopts/cxxopts/include
    ../../external/fmtlib/fmtlib/include/fmt
)
