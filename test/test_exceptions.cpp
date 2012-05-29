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

struct ex : public std::exception, public counted_type<ex>
{
    ex(const char* m): msg(m) {}
    virtual ~ex() throw() {}
    virtual const char* what() const throw() { return msg; }
    const char* msg;
};

struct exception_thrower : counted_type<exception_thrower>
{
    exception_thrower() {}
    exception_thrower(int) { throw ex("exception description"); }
    exception_thrower(int, int) { throw "a string exception"; }
    exception_thrower(int, int, int) { throw 10; }
    int f() { throw ex("exception from a member function"); }
    int g() { throw "a string exception"; }
    int h() { throw 10; }
};

COUNTER_GUARD(exception_thrower);

void test_main(lua_State* L)
{
    using namespace luaponte;

#ifndef LUAPONTE_NO_EXCEPTIONS

    const int start_count = ex::count;

    module(L)
    [
        class_<exception_thrower>("throw")
            .def(constructor<>())
            .def(constructor<int>())
            .def(constructor<int, int>())
            .def(constructor<int, int, int>())
            .def("f", &exception_thrower::f)
            .def("g", &exception_thrower::g)
            .def("h", &exception_thrower::h)
    ];

    DOSTRING_EXPECTED(L, "a = throw(1)", "std::exception: 'exception description'");
    DOSTRING_EXPECTED(L, "a = throw(1,1)", "c-string: 'a string exception'");
    DOSTRING_EXPECTED(L, "a = throw(1,1,1)", "Unknown C++ exception");
    DOSTRING(L, "a = throw()");
    DOSTRING_EXPECTED(L, "a:f()", "std::exception: 'exception from a member function'");
    DOSTRING_EXPECTED(L, "a:g()", "c-string: 'a string exception'");

    DOSTRING_EXPECTED(L, "a:h()", "Unknown C++ exception");
    DOSTRING_EXPECTED(L,
        "obj = throw('incorrect', 'parameters', 'constructor')",
        "No matching overload found, candidates:\n"
        "void __init(luaponte::argument const&,int,int,int)\n"
        "void __init(luaponte::argument const&,int,int)\n"
        "void __init(luaponte::argument const&,int)\n"
        "void __init(luaponte::argument const&)");

    const int end_count = ex::count;
    TEST_CHECK( start_count == end_count );

#endif
}
