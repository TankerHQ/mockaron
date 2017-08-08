project(PackageTest CXX)
cmake_minimum_required(VERSION 3.0)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_executable(mockaron_test test.cpp)
target_link_libraries(mockaron_test CONAN_PKG::mockaron)
