#include <unordered_set>
#include <unordered_map>
#include <cassert>

#include <mockaron/mockaron.hpp>

namespace mockaron
{
namespace detail
{

namespace
{
std::unordered_set<void const*> object_mocks;
std::unordered_map<void const*, any> function_mocks;
}

void mock_impl::add_hook_(char const* name, char const* sig, any h)
{
  _methods[std::string(name) + "##" + sig] = std::move(h);
}

bool is_a_mock(void const* v)
{
  return object_mocks.count(v);
}

void register_mock(void const* p)
{
  assert(!object_mocks.count(p));
  object_mocks.insert(p);
}

void unregister_mock(void const* p)
{
  auto c = object_mocks.erase(p);
  (void)c;
  assert(c);
}

any* get_function_hook(void const* n)
{
  if (!function_mocks.count(n))
    return nullptr;
  else
    return &function_mocks[n];
}

void register_function_mock(void const* n, any h)
{
  assert(!function_mocks.count(n));
  function_mocks[n] = std::move(h);
}

void unregister_function_mock(void const* n)
{
  assert(function_mocks.count(n));
  function_mocks.erase(n);
}

}
}
