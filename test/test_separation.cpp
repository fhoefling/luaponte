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
#include <luaponte/scope.hpp>

namespace {

struct X {};
struct Y {};
    
}

luaponte::scope test_separate_registration()
{
    using namespace luaponte;

    return class_<X>("X")
               .def(constructor<>()),
           class_<Y>("Y")
               .def(constructor<>())
           ;
}

void test_main(lua_State* L)
{
    using namespace luaponte;
    
    module(L)
    [
        namespace_("Z") [
            test_separate_registration()
        ]
    ];

    DOSTRING(L, "x = Z.X()");
    DOSTRING(L, "y = Z.Y()");
}
