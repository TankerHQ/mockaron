#ifndef MOCKARON_TYPETRAITS_HPP
#define MOCKARON_TYPETRAITS_HPP

namespace mockaron
{
namespace detail
{

template <typename T>
struct remove_class_ptr;

#define MOCKARON_EXPAND_WITH_ATTR(macro)                                \
  macro() macro(const) macro(volatile) macro(const volatile)            \
  macro(&) macro(const &) macro(volatile &) macro(const volatile &)     \
  macro(&&) macro(const &&) macro(volatile &&) macro(const volatile &&)

#define MOCKARON_DEFINE_remove_class_ptr(attr)        \
  template <typename R, typename T, typename... Args> \
  struct remove_class_ptr<R (T::*)(Args...) attr>     \
  {                                                   \
    using type = R(Args...) attr;                     \
  };

MOCKARON_EXPAND_WITH_ATTR(MOCKARON_DEFINE_remove_class_ptr)

#undef MOCKARON_DEFINE_remove_class_ptr

template <typename T>
using remove_class_ptr_t = typename remove_class_ptr<T>::type;

template <typename C, typename T>
struct add_class_ptr;

#define MOCKARON_DEFINE_add_class_ptr(attr)           \
  template <typename C, typename R, typename... Args> \
  struct add_class_ptr<C, R(Args...) attr>            \
  {                                                   \
    using type = R (C::*)(Args...) attr;              \
  };

MOCKARON_EXPAND_WITH_ATTR(MOCKARON_DEFINE_add_class_ptr)

#undef MOCKARON_DEFINE_add_class_ptr

template <typename C, typename T>
using add_class_ptr_t = typename add_class_ptr<C, T>::type;

template <typename T>
struct remove_abomination;

#define MOCKARON_DEFINE_remove_abomination(attr)  \
  template <typename R, typename... Args>    \
  struct remove_abomination<R(Args...) attr> \
  {                                          \
    using type = R(Args...);                 \
  };

MOCKARON_EXPAND_WITH_ATTR(MOCKARON_DEFINE_remove_abomination)

#undef MOCKARON_DEFINE_remove_abomination

template <typename T>
using remove_abomination_t = typename remove_abomination<T>::type;

#undef MOCKARON_EXPAND_WITH_ATTR

// GCC doesn't like using typeid on abominable function signatures, so we wrap
// them inside this struct
template <typename T>
struct wrap{};

}
}

#endif
