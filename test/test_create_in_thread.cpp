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

int count = 0;

struct X
{
    X()
    {
        ++count;
    }

    ~X()
    {
        --count;
    }
};

struct Y
{
    virtual ~Y()
    {}
};

struct Y_wrap : Y, luaponte::wrap_base
{};

void test_main(lua_State* L)
{
    using namespace luaponte;

    module(L) [
        class_<X>("X")
            .def(constructor<>()),

        class_<Y, Y_wrap>("Y")
            .def(constructor<>())
    ];

    DOSTRING(L,
        "class 'Y_lua' (Y)\n"
        "  function Y_lua.__init(self)\n"
        "      Y.__init(self)\n"
        "  end\n"
    );

    for (int i = 0; i < 100; ++i)
    {
        lua_State* thread = lua_newthread(L);
        int ref = luaL_ref(L, LUA_REGISTRYINDEX);

        DOSTRING(thread,
            "local x = X()\n"
        );

        DOSTRING(thread,
            "local y = Y_lua()\n"
        );

        luaL_unref(L, LUA_REGISTRYINDEX, ref);
    }
}
