// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <luaponte/value_wrapper.hpp>
#include <luaponte/object.hpp>
#include <boost/mpl/assert.hpp>

struct X_tag;

struct X
{
    typedef X_tag value_wrapper_tag;
};

namespace luaponte {
#ifdef LUAPONTE_USE_VALUE_WRAPPER_TAG
  template<>
  struct value_wrapper_traits<X_tag>
  {
      typedef boost::mpl::true_ is_specialized;
  };
#else
  // used on compilers supporting partial template specialization
  template<>
  struct value_wrapper_traits<X>
  {
      typedef boost::mpl::true_ is_specialized;
  };
#endif

} // namespace luaponte

BOOST_MPL_ASSERT(( luaponte::is_value_wrapper<X> ));
BOOST_MPL_ASSERT_NOT(( luaponte::is_value_wrapper<X&> ));
BOOST_MPL_ASSERT_NOT(( luaponte::is_value_wrapper<X const&> ));

BOOST_MPL_ASSERT(( luaponte::is_value_wrapper_arg<X> ));
BOOST_MPL_ASSERT(( luaponte::is_value_wrapper_arg<X const> ));
BOOST_MPL_ASSERT(( luaponte::is_value_wrapper_arg<X&> ));
BOOST_MPL_ASSERT(( luaponte::is_value_wrapper_arg<X const&> ));
BOOST_MPL_ASSERT_NOT(( luaponte::is_value_wrapper_arg<int> ));
BOOST_MPL_ASSERT_NOT(( luaponte::is_value_wrapper_arg<int[4]> ));

BOOST_MPL_ASSERT(( luaponte::is_value_wrapper_arg<X const&> ));
BOOST_MPL_ASSERT(( luaponte::is_value_wrapper_arg<luaponte::object&> ));
BOOST_MPL_ASSERT(( luaponte::is_value_wrapper_arg<luaponte::object const&> ));

int main()
{
}
