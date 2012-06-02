// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <luaponte/detail/has_get_pointer.hpp>

#include <boost/mpl/assert.hpp>
#include <memory>

namespace lb = luaponte::detail;

namespace test {

struct X
{
};

struct Y
{
};

Y* get_pointer(Y const&);

struct Z : std::enable_shared_from_this<Z> {};
  
} // namespace test

BOOST_MPL_ASSERT(( lb::has_get_pointer<std::unique_ptr<int> > ));
BOOST_MPL_ASSERT(( lb::has_get_pointer<std::shared_ptr<int> > ));
BOOST_MPL_ASSERT(( lb::has_get_pointer<test::Y> ));
BOOST_MPL_ASSERT(( lb::has_get_pointer<char*> ));
BOOST_MPL_ASSERT_NOT(( lb::has_get_pointer<int> ));
BOOST_MPL_ASSERT_NOT(( lb::has_get_pointer<test::X> ));
BOOST_MPL_ASSERT(( lb::has_get_pointer<test::Z*> ));

void test_main(lua_State* L)
{
}
