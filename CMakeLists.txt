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
target_include_directories(mockaron PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

install(TARGETS mockaron
  EXPORT mockaron
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
install(DIRECTORY include
  DESTINATION .
)
install(EXPORT mockaron
  DESTINATION lib/cmake/mockaron
  FILE "mockaron-config.cmake"
  NAMESPACE mockaron::
)

if (NOT MOCKARON_NOTEST)
  add_subdirectory(test)
endif()
