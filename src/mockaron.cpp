#include <unordered_set>
#include <cassert>

#include <mockaron/mockaron.hpp>

namespace mockaron
{
namespace detail
{

namespace
{
std::unordered_set<void const*> mocks;
}

void mock_impl::add_hook_(char const* name, char const* sig, any h)
{
  _methods[std::string(name) + "##" + sig] = std::move(h);
}

bool is_a_mock(void const* v)
{
  return mocks.count(v);
}

void register_mock(void const* p)
{
  assert(!mocks.count(p));
  mocks.insert(p);
}

void unregister_mock(void const* p)
{
  auto c = mocks.erase(p);
  (void)c;
  assert(c);
}

}
}
