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
#include <luaponte/shared_ptr_converter.hpp>

struct X
{
    X(int value)
      : value(value)
    {}

    int value;
};

int get_value(std::shared_ptr<X> const& p)
{
    return p->value;
}

std::shared_ptr<X> filter(std::shared_ptr<X> const& p)
{
    return p;
}

void test_main(lua_State* L)
{
    using namespace luaponte;

    module(L) [
        class_<X>("X")
            .def(constructor<int>()),
        def("get_value", &get_value),
        def("filter", &filter)
    ];

    DOSTRING(L,
        "x = X(1)\n"
        "assert(get_value(x) == 1)\n"
    );

    DOSTRING(L,
        "assert(x == filter(x))\n"
    );
}
