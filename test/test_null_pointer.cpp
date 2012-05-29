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

struct A : counted_type<A>
{
    A* f() { return 0; }
};

A* return_pointer()
{
    return 0;
}

COUNTER_GUARD(A);

void test_main(lua_State* L)
{
    using namespace luaponte;

    module(L)
    [
        class_<A>("A")
            .def(constructor<>())
            .def("f", &A::f),

        def("return_pointer", &return_pointer)
    ];


    DOSTRING(L,
        "e = return_pointer()\n"
        "assert(e == nil)");

    DOSTRING(L,
        "a = A()\n"
        "e = a:f()\n"
        "assert(e == nil)");
}
