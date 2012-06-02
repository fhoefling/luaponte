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
#include <memory>

struct Base
{
    Base(int value)
      : value(value)
    {}

    virtual ~Base()
    {}

    int g() const
    {
        return value;
    }

    int value;
};

struct Derived : Base
{
    Derived()
      : Base(2)
    {}

    int f() const
    {
        return 1;
    }
};

struct Unregistered : Base
{
    Unregistered()
      : Base(3)
    {}
};

std::unique_ptr<Base> make_derived()
{
    return std::unique_ptr<Base>(new Derived);
}

std::unique_ptr<Base> make_unregistered()
{
    return std::unique_ptr<Base>(new Unregistered);
}

void test_main(lua_State* L)
{
    using namespace luaponte;

    module(L) [
        class_<Base>("Base")
            .def("g", &Base::g),
        class_<Derived, Base>("Derived")
            .def("f", &Derived::f),
        def("make_derived", &make_derived),
        def("make_unregistered", &make_unregistered)
    ];

    DOSTRING(L,
        "x = make_derived()\n"
        "assert(x:f() == 1)\n"
    );

    DOSTRING(L,
        "x = make_unregistered()\n"
        "assert(x:g() == 3)\n"
    );
}
