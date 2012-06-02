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
#include <luaponte/class_info.hpp>

#include <memory>

struct Foo
{
    Foo() : m_baz(0) {}
    int m_baz;
};

struct Bar
{
    std::shared_ptr<Foo> getFoo() const { return m_foo; }
    void setFoo( std::shared_ptr<Foo> foo ) {  m_foo = foo; }

    std::shared_ptr<Foo> m_foo;
};

void test_main(lua_State* L)
{
    using namespace luaponte;

    bind_class_info(L);

    module( L )
    [
        class_<Foo, std::shared_ptr<Foo> >( "Foo" )
            .def( constructor<>() )
            .def_readwrite("baz", &Foo::m_baz),
        class_<Bar, std::shared_ptr<Bar> >( "Bar" )
            .def( constructor<>() )
            .property("fooz", &Bar::getFoo, &Bar::setFoo)
            .def_readwrite("foo", &Bar::m_foo)
    ];
    dostring( L, "foo = Foo();");
    dostring( L,    "foo.baz = 42;");
    dostring( L,    "x = Bar();");
    dostring( L,    "x.fooz = foo;");
    dostring( L,    "print(type(x), class_info(x).name);");
    dostring( L,    "print(type(x.fooz), class_info(x.fooz).name);");
    dostring( L,    "print(type(x.foo), class_info(x.foo).name);"); // crashes here);
}
