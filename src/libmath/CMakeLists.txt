set(target_name libmath)

add_library(${target_name} SHARED
  sum.cpp
  ../../include/libmath/sum.hpp)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
