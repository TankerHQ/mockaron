#include "testlib.hpp"

#include <iostream>

#include <mockaron/mockaron.hpp>

My::My(float)
  : _i(424242)
{
}


My::~My()
{
  if (_i != 424242)
    std::terminate();
}

int My::f(int y)
{
  MOCKARON_HOOK(My, f, y);

  return y * 4;
}

int My::g(int const& y)
{
  MOCKARON_HOOK_SIG(int(int const&), My, g, y);

  return y * 4;
}

float My::g(float y)
{
  MOCKARON_HOOK_SIG(float(float), My, g, y);

  return y * 4;
}

int My::h(int y) const
{
  MOCKARON_HOOK(My, h, y);
  return y * 4;
}

int My::i()
{
  MOCKARON_HOOK0(My, i);
  return 0;
}

void My::j()
{
  MOCKARON_HOOK0(My, j);
  throw std::runtime_error("don't call me");
}
