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

using namespace luaponte;

struct abstract
{
    virtual ~abstract() {}
    virtual std::string hello() = 0;
};

COUNTER_GUARD(abstract);

struct concrete : abstract
{
    std::string hello()
    {
        return "test string";
    }
};

struct abstract_wrap : abstract, wrap_base
{
    std::string hello()
    {
        return call_member<std::string>(this, "hello");
    }

    static void default_hello(abstract const&)
    {
        throw std::runtime_error("abstract function");
    }
};

std::string call_hello(abstract& a)
{
    return a.hello();
}

abstract& return_abstract_ref()
{
    static concrete c;
    return c;
}

abstract const& return_const_abstract_ref()
{
    static concrete c;
    return c;
}


void test_main(lua_State* L)
{
    module(L)
    [
        class_<abstract, abstract_wrap>("abstract")
            .def(constructor<>())
            .def("hello", &abstract::hello),

        def("call_hello", &call_hello),
          def("return_abstract_ref", &return_abstract_ref),
          def("return_const_abstract_ref", &return_const_abstract_ref)
    ];

    DOSTRING_EXPECTED(L,
        "x = abstract()\n"
        "x:hello()\n"
      , "std::runtime_error: 'Attempt to call nonexistent function'");

    DOSTRING_EXPECTED(L,
        "call_hello(x)\n"
      , "std::runtime_error: 'Attempt to call nonexistent function'");

    DOSTRING(L,
        "class 'concrete' (abstract)\n"
        "  function concrete:__init()\n"
        "      abstract.__init(self)\n"
        "  end\n"

        "  function concrete:hello()\n"
        "      return 'hello from lua'\n"
        "  end\n");

    DOSTRING(L,
        "y = concrete()\n"
        "y:hello()\n");

    DOSTRING(L, "call_hello(y)\n");

    DOSTRING(L,
        "x = abstract()\n"
        "x.hello = function(self) return 'hello from instance' end\n"
        "print(x.hello)\n"
        "assert(x:hello() == 'hello from instance')\n"
        "assert(call_hello(x) == 'hello from instance')\n"
    );
}
