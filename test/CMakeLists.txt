add_library(testlib STATIC
  testlib.hpp
  testlib.cpp
)
target_link_libraries(testlib mockaron)

add_library(testlibnohook STATIC
  testlib.hpp
  testlib.cpp
)
target_link_libraries(testlibnohook mockaron)
target_compile_definitions(testlibnohook PRIVATE MOCKARON_DISABLE_HOOKS)

add_executable(test_mockaron
  test.cpp
)
target_link_libraries(test_mockaron testlib mockaron)

add_executable(test_mockaron_nohook
  test.cpp
)
target_link_libraries(test_mockaron_nohook testlibnohook mockaron)
target_compile_definitions(test_mockaron_nohook PRIVATE MOCKARON_DISABLE_HOOKS)

add_test(NAME test_mockaron COMMAND test_mockaron)
add_test(NAME test_mockaron_nohook COMMAND test_mockaron_nohook)
