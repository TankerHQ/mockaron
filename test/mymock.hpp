#ifndef TEST_MYMOCK_HPP
#define TEST_MYMOCK_HPP

#include "testlib.hpp"

#include <mockaron/mockaron.hpp>

struct MyMock : public mockaron::mock_impl
{
  MyMock()
  {
    MOCKARON_DECLARE_IMPL(My, f);
    MOCKARON_DECLARE_IMPL_SIG(int(int const&), My, g);
    MOCKARON_DECLARE_IMPL_SIG(float(float), My, g);
    MOCKARON_DECLARE_IMPL(My, h);
    MOCKARON_DECLARE_IMPL(My, i);
    MOCKARON_DECLARE_IMPL(My, j);
    MOCKARON_DECLARE_IMPL(My, k);
  }

  int f(int y)
  {
    return y * 2;
  }

  int g(int y)
  {
    return y * 2;
  }
  float g(float y)
  {
    return y * 2;
  }

  int h(int y)
  {
    return y*2;
  }

  int i()
  {
    return 2;
  }

  void j()
  {
    z = 42;
  }

  My::NoCopy& k()
  {
    return no;
  }

  template <typename T>
  int t(T v)
  {
    return 2 * v;
  }

  int z{0};
  My::NoCopy no;
};

#define CHECK(x)                                     \
  do                                                 \
  {                                                  \
    if (!(x))                                        \
    {                                                \
      std::cerr << "Check failed: " #x << std::endl; \
      ++failure;                                     \
    }                                                \
  } while (false)

#endif
