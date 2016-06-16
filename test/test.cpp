#include <cassert>
#include <iostream>

#include "mymock.hpp"

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
  CHECK(mock.get_mock_impl().z == 42);

  CHECK(real.f(4) == 16);

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
