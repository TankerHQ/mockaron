========
Mockaron
========

What is it?
***********

Mockaron is a mocking library for C++. Unlike a lot of other libraries, mockaron
does not rely on virtual interfaces or template interfaces. It relies on hook
macros that you have to put at the beginning of your functions.

For the moment, mockaron needs C++14 to compile.

What is it not?
***************

Mockaron does *not* define ``EXPECT_CALL``-like macros, but it allows one to
define a mock that relies on another library's mock like googlemock.

Example usage
*************

Mocking a class
---------------

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

Mocking and defining an overloaded methods
------------------------------------------

In case of overloading, you must use the ``_SIG`` variant of the macros and
specify the signature of the method you want to mock:

.. code-block:: cpp

  // to hook the function
  MOCKARON_HOOK_SIG(void(std::string const&), MyClass, MyFunction, str);
  // to define it
  MOCKARON_DECLARE_IMPL_SIG(void(std::string const&), MyClass, myFunction);

Mocking a free function
-----------------------

Similarly to hooking class methods, you must call a macro to hook free
functions:

.. code-block:: cpp

  std::string myFunction(float f)
  {
    MOCKARON_FUNCTION_HOOK(myFunction, f);

    return f == 0 ? "OK" : "Not OK";
  }

Then you can mock that function for the duration of a scope:

.. code-block:: cpp

  void MyTest()
  {
    assert(myFunction(0) == "OK");
    {
      // from this point, until the end of the scope, the function will be
      // mocked
      MOCKARON_SET_FUNCTION_IMPL(myFunction, [](float f){
        return "MOCKED";
      });
      assert(myFunction(0) == "MOCKED");
    }
    assert(myFunction(1) == "Not OK");
  }

Mocking and defining an overloaded free function
------------------------------------------------

To use overloaded free functions with mockaron, you just have to ``static_cast``
the function to the correct function pointer type:

.. code-block:: cpp

  // to hook the function
  MOCKARON_FUNCTION_HOOK(static_cast<int(*)(char)>(myFunction), c);
  // to define the function
  MOCKARON_SET_FUNCTION_IMPL(static_cast<int(*)(char)>(myFunction),
      [](char) { return 0; });

Quick documentation
*******************

About hooks:

- To hook a method, use ``MOCKARON_HOOK``.
- If your method takes no argument, use ``MOCKARON_HOOK0``.
- If your method is overloaded (multiple signatures with the same name), use
  ``MOCKARON_HOOK_SIG`` and give the exact signature as the first argument.
- If your method is overloaded and takes no argument, use
  ``MOCKARON_HOOK_SIG0``

- To hook a free function, use ``MOCKARON_FUNCTION_HOOK``
- If your function takes no argument, use ``MOCKARON_FUNCTION_HOOK0``
- In case of overloading, static_cast the function to the correct function
  pointer type

About mock implementation:

- To define a mock function, use ``MOCKARON_DECLARE_IMPL``
- If your function is overloaded, use ``MOCKARON_DECLARE_IMPL_SIG``

To disable hooking features, you must define ``MOCKARON_DISABLE_HOOKS=1`` when
compiling your library.

FAQ
***

How does it work?
-----------------

There's a global map and an ugly ``reinterpret_cast``. The source code is very
short, you can read it.

What!? Does it invoke undefined behavior?
-----------------------------------------

Probably.

Troubleshooting
***************

Case 1
------

::

  ../test.cpp:28:3: error: address of overloaded function 'f' cannot be static_cast to type '::mockaron::detail::add_class_ptr_t<MyClass, int (const float &)>' (aka 'int (MyClass::*)(const float &)')

This means that you are using one of the ``_SIG`` macros but got the signature
wrong, double check it.

Case 2
------

::

  GetBlobId##N8mockaron6detail4wrapIKFN5boost8optionalIN6Tanker6Common6BlobIdEEERKNS2_10filesystem4pathEEEE is not mocked!

This means that you put a hook on one of the methods of your class but you
didn't provide an implementation for the mock with ``MOCK_SET_IMPL`` or
``MOCK_DECLARE_IMPL``.

The logged name in two parts, you have the name of the function before the
``##`` thing, and the mangled function signature after.
If you demangle it, you can get the full signature::

  $ c++filt -t N8mockaron6detail4wrapIKFN5boost8optionalIN6Tanker6Common6BlobIdEEERKNS2_10filesystem4pathEEEE
  mockaron::detail::wrap<boost::optional<Tanker::Common::BlobId> (boost::filesystem::path const&) const>

Ignore the ``mockaron::detail::wrap<>`` part and you get the signature of the
method.
