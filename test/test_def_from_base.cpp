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

struct V
{
    int f(int,int) 
    {
        return 2; 
    }
};

struct W : V
{};

void test_main(lua_State* L)
{
    using namespace luaponte;

    module(L)
    [
        class_<W>("W")
          .def(constructor<>())
          .def("f", &V::f)
    ];

    DOSTRING(L,
        "x = W()\n"
        "assert(x:f(1,2) == 2)\n"
    );
}
