cmake_minimum_required(VERSION 3.5)
project(mockaron)

enable_testing()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-Wall -Wextra -pedantic)
endif()

add_library(mockaron STATIC
  include/mockaron/detail/typetraits.hpp
  include/mockaron/mockaron.hpp
  src/mockaron.cpp
)
target_include_directories(mockaron PUBLIC include)

add_subdirectory(test)
