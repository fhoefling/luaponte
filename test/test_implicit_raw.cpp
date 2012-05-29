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

void raw_function(lua_State* L)
{}

void raw_function2(int, lua_State* L, int)
{}

void test_main(lua_State* L)
{
    using namespace luaponte;

    module(L) [
        def("raw_function", &raw_function),
        def("raw_function2", &raw_function2)
    ];

    DOSTRING(L,
        "raw_function()\n"
        "raw_function2(1,2)\n"
    );
}
