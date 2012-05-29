// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <luaponte/detail/typetraits.hpp>
#include <luaponte/detail/pointee_sizeof.hpp>
#include <boost/static_assert.hpp>

using namespace luaponte;
using namespace luaponte::detail;

struct tester {};
struct lua_State;

int test_main(lua_State*)
{
    BOOST_STATIC_ASSERT(is_nonconst_reference<int&>::value);
    BOOST_STATIC_ASSERT(!is_nonconst_reference<const int&>::value);
    BOOST_STATIC_ASSERT(is_nonconst_reference<tester&>::value);
    BOOST_STATIC_ASSERT(!is_nonconst_reference<const tester&>::value);

    BOOST_STATIC_ASSERT(!is_const_reference<int&>::value);
    BOOST_STATIC_ASSERT(is_const_reference<const int&>::value);
    BOOST_STATIC_ASSERT(!is_const_reference<tester&>::value);
    BOOST_STATIC_ASSERT(is_const_reference<const tester&>::value);

    BOOST_STATIC_ASSERT(!is_const_pointer<int*>::value);
    BOOST_STATIC_ASSERT(is_const_pointer<const int*>::value);
    BOOST_STATIC_ASSERT(!is_const_pointer<tester*>::value);
    BOOST_STATIC_ASSERT(is_const_pointer<const tester*>::value);

    BOOST_STATIC_ASSERT(is_nonconst_pointer<int*>::value);
    BOOST_STATIC_ASSERT(!is_nonconst_pointer<const int*>::value);
    BOOST_STATIC_ASSERT(is_nonconst_pointer<tester*>::value);
    BOOST_STATIC_ASSERT(!is_nonconst_pointer<const tester*>::value);

    BOOST_STATIC_ASSERT(!is_const_reference<const tester>::value);

    return 0;
}
