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
#include <luaponte/adopt_policy.hpp>

struct X
{
    virtual ~X()
    {}
};

struct X_wrap : X, luaponte::wrap_base
{};

X* make()
{
    return new X;
}

void take(X* p)
{
    delete p;
}

void test_main(lua_State* L)
{
    using namespace luaponte;

    module(L) [
        class_<X, X_wrap>("X"),
        def("make", &make, adopt(result)),
        def("take", &take, adopt(_1))
    ];

    DOSTRING(L,
        "take(make())\n"
    );
}
