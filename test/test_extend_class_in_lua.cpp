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

struct CppClass
{
    int f(int x)
    {
        return x;
    }
};

void test_main(lua_State* L)
{
    using namespace luaponte;

    module(L) [
        class_<CppClass>("CppClass")
          .def(constructor<>())
          .def("f", &CppClass::f)
    ];

    DOSTRING(L,
        "function CppClass:f_in_lua(x)\n"
        "    return self:f(x) * 2\n"
        "end\n"
    );

    DOSTRING(L,
        "x = CppClass()\n"
        "assert(x:f(1) == 1)\n"
        "assert(x:f_in_lua(1) == 2)\n"
    );
}
