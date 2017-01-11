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

My::NoCopy& My::k()
{
  MOCKARON_HOOK0(My, k);
  return _no;
}

char const* My::l(int xx)
{
  MOCKARON_FUNCTION_HOOK(l, xx);
  (void)xx;
  return "fortytwo";
}

bool My::m()
{
  MOCKARON_FUNCTION_HOOK0(m);
  return false;
}

int My::n(int xx)
{
  MOCKARON_FUNCTION_HOOK(static_cast<int(*)(int)>(n), xx);
  return xx * 2;
}

float My::n(float xx)
{
  MOCKARON_FUNCTION_HOOK(static_cast<float(*)(float)>(n), xx);
  return xx * 2;
}
