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

struct A : counted_type<A> 
{ virtual ~A() {} };

struct B : A, counted_type<B>  
{};


struct enum_placeholder {};
typedef enum { VAL1 = 1, VAL2 = 2 } LBENUM_t;
LBENUM_t enum_by_val(LBENUM_t e)    { return e; }
LBENUM_t enum_by_const_ref(const LBENUM_t &e)   { return e; }



struct test_implicit : counted_type<test_implicit>
{
    char const* f(A*) { return "f(A*)"; }
    char const* f(B*) { return "f(B*)"; }
};

struct char_pointer_convertable
  : counted_type<char_pointer_convertable>
{
    operator const char*() const { return "foo!"; }
};

void func(const char_pointer_convertable& f)
{
}

void not_convertable(boost::shared_ptr<A>)
{
    TEST_CHECK(false);
}

int f(int& a)
{
    return a;
}

COUNTER_GUARD(A);
COUNTER_GUARD(B);
COUNTER_GUARD(test_implicit);
COUNTER_GUARD(char_pointer_convertable);

void test_main(lua_State* L)
{
    using namespace luaponte;

    typedef char const*(test_implicit::*f1)(A*);
    typedef char const*(test_implicit::*f2)(B*);

    module(L)
    [
        class_<A>("A")
            .def(constructor<>()),
    
        class_<B, A>("B")
            .def(constructor<>()),
    
        class_<test_implicit>("test")
            .def(constructor<>())
            .def("f", (f1)&test_implicit::f)
            .def("f", (f2)&test_implicit::f),

        class_<char_pointer_convertable>("char_ptr")
            .def(constructor<>()),

        class_<enum_placeholder>("LBENUM")
            .enum_("constants")
            [
                value("VAL1", VAL1),
                value("VAL2", VAL2)
            ],
        def("enum_by_val", &enum_by_val),
        def("enum_by_const_ref", &enum_by_const_ref),

        def("func", &func),
        def("no_convert", &not_convertable),
        def("f", &f)
    ];

    DOSTRING(L, "a = A()");
    DOSTRING(L, "b = B()");
    DOSTRING(L, "t = test()");

    DOSTRING(L, "assert(t:f(a) == 'f(A*)')");
    DOSTRING(L, "assert(t:f(b) == 'f(B*)')");

    DOSTRING(L, 
        "a = char_ptr()\n"
        "func(a)");

    DOSTRING(L, "assert(LBENUM.VAL1 == 1)");
    DOSTRING(L, "assert(LBENUM.VAL2 == 2)");
    DOSTRING(L, "assert(enum_by_val(LBENUM.VAL1) == LBENUM.VAL1)");
    DOSTRING(L, "assert(enum_by_val(LBENUM.VAL2) == LBENUM.VAL2)");
    DOSTRING(L, "assert(enum_by_const_ref(LBENUM.VAL1) == LBENUM.VAL1)");
    DOSTRING(L, "assert(enum_by_const_ref(LBENUM.VAL2) == LBENUM.VAL2)");

    DOSTRING_EXPECTED(L,
        "a = A()\n"
        "no_convert(a)",
        ("No matching overload found, candidates:\n"
        "void no_convert(custom ["
        + type_id(typeid(boost::shared_ptr<A>)).name() + "])").c_str());

    DOSTRING_EXPECTED(L,
        "a = nil\n"
        "f(a)",
        "No matching overload found, candidates:\n"
        "int f(int&)");
}
