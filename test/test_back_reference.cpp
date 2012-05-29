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
#include <boost/shared_ptr.hpp>

using namespace luaponte;

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace luaponte
{
  using boost::get_pointer;
}
#endif

struct base0
{
    virtual ~base0() {}
};

struct base_wrap0 : base0, wrap_base
{};

struct base1
{
    virtual ~base1() {}
};

struct base_wrap1 : base1, wrap_base
{};

base0* filter0(base0* p)
{
    return p;
}

boost::shared_ptr<base1> filter1(boost::shared_ptr<base1> const& p)
{
    return p;
}

void test_main(lua_State* L)
{
    module(L)
    [
        class_<base0, base_wrap0>("base0")
          .def(constructor<>()),
        def("filter0", &filter0),

        class_<base1, base_wrap1, boost::shared_ptr<base1> >("base1")
          .def(constructor<>()),
        def("filter1", &filter1)
    ];

    DOSTRING(L,
        "class 'derived0' (base0)\n"
        "  function derived0:__init()\n"
        "    base0.__init(self)\n"
        "  end\n"

        "class 'derived1' (base1)\n"
        "  function derived1:__init()\n"
        "    base1.__init(self)\n"
        "  end\n"
    );

    DOSTRING(L,
        "x = derived0()\n"
        "y = filter0(x)\n"
        "assert(x == y)\n"
    );

    DOSTRING(L,
        "x = derived1()\n"
        "y = filter1(x)\n"
        "assert(x == y)\n"
    );
}
