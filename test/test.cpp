#include <cassert>
#include <cstring>
#include <iostream>

#if MOCKARON_DISABLE_HOOKS
#include "mymock.hpp"
#else
#define MYCLASS_IS_MOCKED
#include "mymock.hpp"

template <typename T>
int My::t(T v)
{
  MOCKARON_HOOK_TEMPLATE(My, t, MyMock, v);
  return this->t_(v);
}
#endif

int testnohook()
{
  int failure = 0;
  My real(3.14);

  mockaron::mock<My, MyMock> mock;

  CHECK(real.f(4) == 16);
  CHECK(mock.get().f(4) == 16);
  CHECK(mock.get().g(4) == 16);
  CHECK(mock.get().g(2.0f) == 8.0f);
  CHECK(mock.get().h(2) == 8);
  CHECK(mock.get().i() == 0);
  CHECK(mock.get_mock_impl().z == 0);
  CHECK(mock.get().t(4) == 12);
  CHECK(mock.get().t(3.0) == 9);

  {
    MOCKARON_SET_FUNCTION_IMPL(My::l, [](int i, char) {
      if (i == 8)
        return "roflol";
      return (char const*)nullptr;
    });
    MOCKARON_SET_FUNCTION_IMPL(My::m, []() { return true; });
    MOCKARON_SET_FUNCTION_IMPL(static_cast<int (*)(int)>(My::n),
                               [](int i) { return i * 4; });
    MOCKARON_SET_FUNCTION_IMPL(static_cast<float (*)(float)>(My::n),
                               [](float i) { return i * 8; });

    CHECK(!std::strcmp(My::l(8, 0), "fortytwo"));
    CHECK(!My::m());
    CHECK(My::n(2) == 4);
    CHECK(My::n(2.f) == 4.f);
  }

  CHECK(real.f(4) == 16);

  return failure;
}

int test()
{
  int failure = 0;
  My real(3.14);
  CHECK(real.f(4) == 16);

  mockaron::mock<My, MyMock> mock;

  CHECK(mock.get().f(4) == 8);
  CHECK(mock.get().g(4) == 8);
  CHECK(mock.get().g(2.0f) == 4.0f);
  CHECK(mock.get().h(2) == 4);
  CHECK(mock.get().i() == 2);
  mock.get().j();
  CHECK(&mock.get().k() == &mock.get_mock_impl().no);
  CHECK(mock.get_mock_impl().z == 42);
  CHECK(mock.get().t(4) == 8);
  CHECK(mock.get().t(3.0) == 6);

  CHECK(real.f(4) == 16);

  CHECK(!std::strcmp(My::l(8, 0), "fortytwo"));
  CHECK(!My::m());
  CHECK(My::n(2) == 4);
  CHECK(My::n(2.f) == 4.f);

  {
    MOCKARON_SET_FUNCTION_IMPL(My::l, [](int i, char) {
      if (i == 8)
        return "roflol";
      return (char const*)nullptr;
    });
    CHECK(!std::strcmp(My::l(8, 0), "roflol"));
  }
  {
    MOCKARON_SET_FUNCTION_IMPL(My::m, []() { return true; });
    CHECK(My::m());
  }
  {
    MOCKARON_SET_FUNCTION_IMPL(static_cast<int (*)(int)>(My::n),
                               [](int i) { return i * 4; });
    MOCKARON_SET_FUNCTION_IMPL(static_cast<float (*)(float)>(My::n),
                               [](float i) { return i * 8; });
    CHECK(My::n(2) == 8);
    CHECK(My::n(2.f) == 16.f);
  }

  return failure;
}

int main()
{
  int failure =
#if MOCKARON_DISABLE_HOOKS
    testnohook();
#else
    test();
#endif

  if (!failure)
    std::cerr << "Success" << std::endl;

  return failure;
}
