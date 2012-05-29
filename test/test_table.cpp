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

void f(luaponte::table<> const& x)
{
    TEST_CHECK(luaponte::type(x) == LUA_TTABLE);
}

void g(luaponte::table<luaponte::argument> const& x)
{
    TEST_CHECK(luaponte::type(x) == LUA_TTABLE);
}

void test_main(lua_State* L)
{
    using namespace luaponte;

    module(L) [
        def("f", &f),
        def("g", &g)
    ];

    DOSTRING(L,
        "f {1,2,3}\n"
        "g {1,2,3}\n"
    );

    DOSTRING_EXPECTED(L,
        "f(1)\n",
        "No matching overload found, candidates:\n"
        "void f(table const&)"
    );

    DOSTRING_EXPECTED(L,
        "g(1)\n",
        "No matching overload found, candidates:\n"
        "void g(table const&)"
    );
}
