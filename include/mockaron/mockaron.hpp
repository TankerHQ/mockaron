#ifndef MOCKARON_HPP
#define MOCKARON_HPP

#include <mockaron/detail/typetraits.hpp>

#include <iostream>
#include <unordered_map>
#include <string>
#include <typeinfo>
#include <functional>

/** Set a function implementation for a mock
 *
 * \param cl the class to mock
 * \param func function to mock
 * \param hndl functor to run when the function is called
 */
#define MOCKARON_SET_IMPL(cl, func, hndl)                           \
  mock_impl::add_hook<                                              \
      ::mockaron::detail::remove_class_ptr_t<decltype(&cl::func)>>( \
      #func, hndl)

/** Set a function implementation for a mock in presence of overload
 *
 * Same as MOCKARON_SET_IMPL with support for overload.
 *
 * \param sig the signature of the function to mock
 *
 * This macro will trigger a compilation error if there is no such signature for
 * the given function.
 */
#define MOCKARON_SET_IMPL_SIG(sig, cl, func, hndl)                   \
  do                                                                 \
  {                                                                  \
    (void)static_cast<::mockaron::detail::add_class_ptr_t<cl, sig>>( \
        &cl::func);                                                  \
    mock_impl::add_hook<sig>(#func, hndl);                           \
  } while (0)

/** Declare an implementation of a function
 *
 * \param cl the class to mock
 * \param func the function to mock
 */
#define MOCKARON_DECLARE_IMPL(cl, func)                       \
  MOCKARON_SET_IMPL(cl, func, [this](auto&&... args) {        \
    return this->func(std::forward<decltype(args)>(args)...); \
  });

/** Declare an implementation of a function in presence of overload
 *
 * Same as MOCKARON_DECLARE_IMPL with support for overload.
 *
 * \param sig the signature of the function to mock
 */
#define MOCKARON_DECLARE_IMPL_SIG(sig, cl, func)                \
  MOCKARON_SET_IMPL_SIG(sig, cl, func, [this](auto&&... args) { \
    return this->func(std::forward<decltype(args)>(args)...);   \
  });

#if MOCKARON_DISABLE_HOOKS
#define MOCKARON_HOOK(...)
#define MOCKARON_HOOK0(...)
#define MOCKARON_HOOK_SIG(...)
#define MOCKARON_HOOK_SIG0(...)
#else
/** Hook a function with mockaron
 *
 * \param cl the class
 * \param func the function
 * \param ... the arguments received
 */
#define MOCKARON_HOOK(cl, func, ...)                               \
  MOCKARON_HOOK_SIG(                                               \
      ::mockaron::detail::remove_class_ptr_t<decltype(&cl::func)>, \
      cl,                                                          \
      func,                                                        \
      __VA_ARGS__)

/** Hook a function without arguments with mockaron
 *
 * Same as MOCKARON_HOOK for function without arguments.
 */
#define MOCKARON_HOOK0(cl, func) \
  MOCKARON_HOOK_SIG0(            \
      ::mockaron::detail::remove_class_ptr_t<decltype(&cl::func)>, cl, func)

/// Private macro
#define MOCKARON_HOOK_SIG_(sig, cl, func, exp)                          \
  do                                                                    \
  {                                                                     \
    (void)static_cast<::mockaron::detail::add_class_ptr_t<cl, sig>>(    \
        &cl::func);                                                     \
    if (!::mockaron::detail::is_a_mock(this))                           \
      break;                                                            \
    return ::mockaron::detail::run_hook<sig>(                           \
        *reinterpret_cast<::mockaron::detail::mock_impl* const*>(this), \
        #func exp);                                                     \
  } while (0)

/// Private macro
#define MOCKARON_NOTHING
/// Private macro
#define MOCKARON_EXPAND(...) , __VA_ARGS__

/** Hook a function in prensence of overload
 *
 * Same as MOCKARON_HOOK with support for overload.
 *
 * \param sig the signature of the function to mock
 */
#define MOCKARON_HOOK_SIG(sig, cl, func, ...) \
  MOCKARON_HOOK_SIG_(sig, cl, func, MOCKARON_EXPAND(__VA_ARGS__))

/** Hook a function without argument in presence of overload
 *
 * Same as MOCKARON_HOOK_SIG for function without arguments.
 */
#define MOCKARON_HOOK_SIG0(sig, cl, func) \
  MOCKARON_HOOK_SIG_(sig, cl, func, MOCKARON_NOTHING)
#endif

namespace mockaron
{
namespace detail
{

class any
{
public:
  any() = default;
  template <typename T>
  any(T&& v)
  {
    set(std::forward<T>(v));
  }
  any(any&& o)
    : _ptr(o._ptr),
      _deleter(std::move(o._deleter))
  {
    o._ptr = nullptr;
  }
  any& operator=(any&& o)
  {
    _ptr = o._ptr;
    _deleter = std::move(o._deleter);
    o._ptr = nullptr;
    return *this;
  }
  ~any()
  {
    if (_ptr)
      _deleter(_ptr);
  }

  template <typename T>
  void set(T&& v)
  {
    _ptr = new std::decay_t<T>(std::forward<T>(v));
    _deleter = [](void* ptr) { delete static_cast<T*>(ptr); };
  }

  template <typename T>
  T& get()
  {
    return *static_cast<T*>(_ptr);
  }

private:
  void* _ptr{nullptr};
  std::function<void(void*)> _deleter;
};

class mock_impl
{
public:
  virtual ~mock_impl() = default;

protected:
  template <typename Sig, typename H>
  void add_hook(char const* name, H&& handler)
  {
    add_hook_(name,
              typeid(wrap<Sig>).name(),
              any(std::function<remove_abomination_t<Sig>>(
                  std::forward<H>(handler))));
  }

private:
  using map_type = std::unordered_map<std::string, any>;
  map_type _methods;

  void add_hook_(char const* name, char const* sig, any h);

  template <typename Sig, typename... Args>
  friend auto run_hook(mock_impl* mi, char const* name, Args&&... args);
};

template <typename Sig, typename... Args>
auto run_hook(mock_impl* mi, char const* name, Args&&... args)
{
  auto const n =
      std::string(name) + "##" + typeid(wrap<Sig>).name();
  if (!mi->_methods.count(n))
  {
    std::cerr << n << " is not mocked! See README for more information." <<
      std::endl;
    std::terminate();
  }
  return mi->_methods[n].get<std::function<remove_abomination_t<Sig>>>()(
      std::forward<Args>(args)...);
}

void register_mock(void const* p);
void unregister_mock(void const* p);
bool is_a_mock(void const* v);

}

/// Class to inherit from to make a mock
using mock_impl = detail::mock_impl;

/** Class to instantiate to create a mock
 *
 * \tparam T the class to mock
 * \tparam M the mock implementation
 */
template <typename T, typename M>
class mock
{
public:
  mock(mock const&) = delete;
  mock& operator=(mock const&) = delete;
  mock(mock&&) = delete;
  mock& operator=(mock&&) = delete;

  mock()
  {
    detail::register_mock(&_p);
    _p = new M();
  }
  ~mock()
  {
    delete _p;
    detail::unregister_mock(&_p);
  }
  T& get()
  {
    return *get_fake();
  }
  M& get_mock_impl()
  {
    return *static_cast<M*>(_p);
  }

private:
  detail::mock_impl* _p;

  T* get_fake()
  {
    return reinterpret_cast<T*>(&_p);
  }
};

}

#endif
