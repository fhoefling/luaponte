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

void test_main(lua_State* L)
{
    DOSTRING(L, "x = 4294967295");

    unsigned int x = luaponte::object_cast<unsigned int>(
        luaponte::globals(L)["x"]);

    unsigned int y = 4294967295UL;

    TEST_CHECK(x == y);
}
