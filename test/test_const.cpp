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

struct A
{
    const A* f() { return this; }

    int g1() const { return 1; }
    int g2() { return 2; }
};

void test_main(lua_State* L)
{
    using namespace luaponte;

    module(L)
    [
        class_<A>("A")
            .def(constructor<>())
            .def("f", &A::f)
            .def("g", &A::g1)
            .def("g", &A::g2)
    ];

    DOSTRING(L, "a = A()");
    DOSTRING(L, "assert(a:g() == 2)");

    DOSTRING(L, "a2 = a:f()");
    DOSTRING(L, "assert(a2:g() == 1)");
}
