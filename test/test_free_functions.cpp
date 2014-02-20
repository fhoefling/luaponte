// Luaponte library

// Copyright (c) 2011-2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <luaponte/luaponte.hpp>
#include <luaponte/adopt_policy.hpp>

struct base : counted_type<base>
{
    int f()
    {
        return 5;
    }
};

COUNTER_GUARD(base);

int f(int x)
{
    return x + 1;
}

int f(int x, int y)
{
    return x + y;
}

base* create_base()
{
    return new base();
}

void test_value_converter(const std::string str)
{
    TEST_CHECK(str == "converted string");
}

void test_pointer_converter(const char* const str)
{
    TEST_CHECK(std::strcmp(str, "converted string") == 0);
}

struct copy_me
{
};

void take_by_value(copy_me m)
{
}

int function_should_never_be_called(lua_State* L)
{
    lua_pushnumber(L, 10);
    return 1;
}

void test_main(lua_State* L)
{
    using namespace luaponte;

    lua_pushcclosure(L, &function_should_never_be_called, 0);
    lua_setglobal(L, "f");

    DOSTRING(L, "assert(f() == 10)");

    module(L)
    [
        class_<copy_me>("copy_me")
            .def(constructor<>()),

        class_<base>("base")
            .def("f", &base::f),


        def("by_value", &take_by_value),

        def("f", (int(*)(int)) &f),
        def("f", (int(*)(int, int)) &f),
        def("create", &create_base, adopt(return_value))
//        def("set_functor", &set_functor)

#if !(BOOST_MSVC < 1300)
        ,
        def("test_value_converter", &test_value_converter),
        def("test_pointer_converter", &test_pointer_converter)
#endif

    ];

    DOSTRING(L,
        "e = create()\n"
        "assert(e:f() == 5)");

    DOSTRING(L, "assert(f(7) == 8)");

    DOSTRING(L, "assert(f(3, 9) == 12)");

//    DOSTRING(L, "set_functor(function(x) return x * 10 end)");

//    TEST_CHECK(functor_test(20) == 200);

//    DOSTRING(L, "set_functor(nil)");

    DOSTRING(L, "function lua_create() return create() end");
    base* ptr = call_function<base*>(L, "lua_create") [ adopt(result) ];
    delete ptr;

#if !(BOOST_MSVC < 1300)
    DOSTRING(L, "test_value_converter('converted string')");
    DOSTRING(L, "test_pointer_converter('converted string')");
#endif

    DOSTRING_EXPECTED(L, "f('incorrect', 'parameters')",
        "No matching overload found, candidates:\n"
        "int f(int,int)\n"
        "int f(int)");


    DOSTRING(L, "function failing_fun() error('expected error message') end");
    try
    {
        call_function<void>(L, "failing_fun");
        TEST_ERROR("function didn't fail when it was expected to");
    }
    catch(luaponte::error const& e)
    {
        // LuaJIT 2.0 and Lua >= 5.2 will return expected1
        std::string expected52("[string \"function failing_fun() error('expected error ...\"]:1: expected error message");
        // Lua < 5.2 will return expected2
        std::string expected51("[string \"function failing_fun() error('expected erro...\"]:1: expected error message");
        std::string result(lua_tostring(L, -1));
        if (result != expected51 && result != expected52)
        {
            TEST_ERROR("function failed with unexpected error message");
        }

        lua_pop(L, 1);
    }

}
