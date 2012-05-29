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

struct simple_class : counted_type<simple_class>
{
    static int feedback;

    void f()
    {
        feedback = 1;
    }

    void f(int, int) {}
    void f(std::string a)
    {
        const char str[] = "foo\0bar";
        if (a == std::string(str, sizeof(str)-1))
            feedback = 2;
    }

    std::string g()
    {
        const char str[] = "foo\0bar";
        return std::string(str, sizeof(str)-1);
    }

};

int simple_class::feedback = 0;
    
COUNTER_GUARD(simple_class);

void test_main(lua_State* L)
{
    using namespace luaponte;

    typedef void(simple_class::*f_overload1)();
    typedef void(simple_class::*f_overload2)(int, int);
    typedef void(simple_class::*f_overload3)(std::string);

    module(L)
    [
        class_<simple_class>("simple")
            .def(constructor<>())
            .def("f", (f_overload1)&simple_class::f)
            .def("f", (f_overload2)&simple_class::f)
            .def("f", (f_overload3)&simple_class::f)
            .def("g", &simple_class::g)
    ];

    DOSTRING(L,
        "class 'simple_derived' (simple)\n"
        "  function simple_derived:__init() simple.__init(self) end\n"
        "a = simple_derived()\n"
        "a:f()\n");
    TEST_CHECK(simple_class::feedback == 1);

    DOSTRING(L, "a:f('foo\\0bar')");
    TEST_CHECK(simple_class::feedback == 2);

    DOSTRING(L,
        "b = simple_derived()\n"
        "a.foo = 'yo'\n"
        "assert(b.foo == nil)");

    DOSTRING(L,
        "simple_derived.foobar = 'yi'\n"
        "assert(b.foobar == 'yi')\n"
        "assert(a.foobar == 'yi')\n");

    simple_class::feedback = 0;

    DOSTRING_EXPECTED(L, "a:f('incorrect', 'parameters')",
        "No matching overload found, candidates:\n"
        "void f(simple&,std::string)\n"
        "void f(simple&,int,int)\n"
        "void f(simple&)");

    DOSTRING(L, "if a:g() == \"foo\\0bar\" then a:f() end");
    TEST_CHECK(simple_class::feedback == 1);
}
