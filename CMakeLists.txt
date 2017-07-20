cmake_minimum_required(VERSION 3.4)
project(mockaron)

option(BUILD_SHARED_LIBS "Build as a shared library" OFF)

if (NOT MOCKARON_NOTEST)
  enable_testing()
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-Wall -Wextra -pedantic)
endif()

add_library(mockaron
  include/mockaron/detail/typetraits.hpp
  include/mockaron/mockaron.hpp
  src/mockaron.cpp
)
target_include_directories(mockaron PUBLIC include)

if (NOT MOCKARON_NOTEST)
  add_subdirectory(test)
endif()
