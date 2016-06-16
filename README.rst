========
Mockaron
========

What is it?
***********

Mockaron is a mocking library for C++. Unlike a lot of other libraries, mockaron
does not rely on virtual interfaces or template interfaces. Mockaron does *not*
define ``EXPECT_CALL``-like macros, but it allows one to define a mock that relies
on another library's mock like googlemock.

For the moment, mockaron needs C++14 to compile.

Example usage
*************

To allow mockaron to intercept function calls, you must insert a hook macro at
the beginning of the function you want to mock in the library you want to test:

.. code-block:: cpp

  int MyClass::myFunction(int arg1, std::string const& arg2)
  {
    MOCKARON_HOOK(MyClass, myFunction, arg1, arg2);

    return arg1 + arg2.size();
  }

This macro allows mockaron to intercept the call. The code after the macro will
not be executed when the object is mocked. When compiling in release mode, it is
possible to make the macro expand to nothing.

You then need to define your mock. Here is one, relying on googlemock:

.. code-block:: cpp

  class MyClassMock : public mockaron::mock_impl
  {
  public:
    // You must define a default constructor
    MyClassMock()
    {
      // You must call this macro for all function you want to mock, it will
      // forward the call to this->myFunction().
      MOCKARON_DECLARE_IMPL(MyClass, myFunction);
    }

    MOCK_METHOD2(myFunction, int(int, std::string const&));
  };

Finally, you can use mocks inside your test:

.. code-block:: cpp

  mockaron::mock<MyClass, MyClassMock> handler;
  MyClass& mockedobject = handler.get();
  MyClassMock& mockimpl = handler.get_mock_impl();

  EXPECT_CALL(mockimpl, myFunction(42, "stuff"))
      .WillOnce(testing::Return(10));

  assert(mockedobject.myFunction(42, "stuff") == 10);

Note that ``mockaron::mock`` will not instantiate a real ``MyClass``, so it does
not need it to be default-constructible.

Quick documentation
*******************

About hooks:

- To hook a function, use ``MOCKARON_HOOK``.
- If your function takes no argument, use ``MOCKARON_HOOK0``.
- If your function is overloaded (multiple signatures with the same name), use
  ``MOCKARON_HOOK_SIG`` and give the exact signature as the first argument.
- If your function is overloaded and takes no argument, use
  ``MOCKARON_HOOK_SIG0``

About mock implementation:

- To define a mock function, use ``MOCKARON_DECLARE_IMPL`` or
  ``MOCKARON_SET_IMPL``
- If your function is overloaded, use ``MOCKARON_DECLARE_IMPL_SIG`` or
  ``MOCKARON_SET_IMPL_SIG``

To disable hooking features, you must define ``MOCKARON_DISABLE_HOOKS=1`` when
compiling your library.

Troubleshooting
***************

::

  ../test.cpp:28:3: error: address of overloaded function 'f' cannot be static_cast to type '::mockaron::detail::add_class_ptr_t<MyClass, int (const float &)>' (aka 'int (MyClass::*)(const float &)')

This means that you are using one of the ``_SIG`` macros but got the signature
wrong, double check it.
