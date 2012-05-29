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
#include <luaponte/exception_handler.hpp>

struct my_exception {};

void translate_my_exception(lua_State* L, my_exception const&)
{
    lua_pushstring(L, "my_exception");
}

struct derived_std_exception : std::exception
{
    char const* what() const throw()
    {
        return "derived_std_exception";
    }
};

void translate_derived_exception(lua_State* L, derived_std_exception const&)
{
    lua_pushstring(L, "derived_std_exception");
}

void raise()
{
    throw my_exception();
}

void raise_derived()
{
    throw derived_std_exception();
}

void test_main(lua_State* L)
{
    using namespace luaponte;

    register_exception_handler<my_exception>(&translate_my_exception);

    module(L) [
        def("raise", &raise),
        def("raise_derived", &raise_derived)
    ];

    DOSTRING(L,
        "status, msg = pcall(raise)\n"
        "assert(status == false)\n"
        "assert(msg == 'my_exception')\n");

    DOSTRING(L,
        "status, msg = pcall(raise_derived)\n"
        "assert(status == false)\n"
        "assert(msg == 'std::exception: \\'derived_std_exception\\'')\n");

    register_exception_handler<derived_std_exception>(&translate_derived_exception);

    DOSTRING(L,
        "status, msg = pcall(raise_derived)\n"
        "assert(status == false)\n"
        "assert(msg == 'derived_std_exception')\n");
}
