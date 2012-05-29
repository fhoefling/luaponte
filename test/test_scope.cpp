// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <luaponte/luaponte.hpp>

int f() { return 1; }
int f_(int a) { return 2; }
int f__(int a) { return 3; }
int g() { return 4; }
int g_(int) { return 5; }
int h() { return 6; }

struct test_class : counted_type<test_class>
{
    test_class()
      : test(1)
    {}
    int test;
};

struct test_class2 : counted_type<test_class2>
{
    test_class2() {}
    int string_string(std::string const& s1, std::string const& s2)
    { return 1; }
};

COUNTER_GUARD(test_class);
COUNTER_GUARD(test_class2);

void test_main(lua_State* L)
{
    using namespace luaponte;

    module(L)
    [
        class_<test_class2>("test_class2")
                .def(constructor<>())
                .def("string_string", &test_class2::string_string)
    ];

    module(L, "test")
    [
        class_<test_class>("test_class")
            .def(constructor<>())
            .def_readonly("test", &test_class::test)
            .scope
            [
                def("inner_fun", &f)
            ]
            .def("inner_fun2", &f) // this should become static
            .enum_("vals")
            [
                value("val1", 1),
                value("val2", 2)
            ],

        def("f", &f),
        def("f", &f_),

        namespace_("inner")
        [
            def("g", &g),
            def("f", &f__)
        ],

        namespace_("inner")
        [
            def("g", &g_)
        ]

    ];

    module(L, "test")
    [
        namespace_("inner")
        [
            def("h", &h)
        ]
    ];


    DOSTRING(L, "assert(test.f() == 1)");
    DOSTRING(L, "assert(test.f(3) == 2)");
    DOSTRING(L, "assert(test.test_class.inner_fun() == 1)");
    DOSTRING(L,
        "a = test.test_class()\n"
        "assert(a.test == 1)");
    DOSTRING(L, "assert(a.inner_fun2() == 1)"); // free function
    DOSTRING(L,
        "b = test.test_class.val2\n"
        "assert(b == 2)");
    DOSTRING(L, "assert(test.inner.g() == 4)");
    DOSTRING(L, "assert(test.inner.g(7) == 5)");
    DOSTRING(L, "assert(test.inner.f(4) == 3)");
    DOSTRING(L, "assert(test.inner.h() == 6)");
}
