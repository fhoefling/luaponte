// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_HAS_GET_POINTER_HPP
#define LUAPONTE_DETAIL_HAS_GET_POINTER_HPP

#include <boost/type_traits/add_reference.hpp>

#ifndef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
# include <memory>
#endif

namespace luaponte {
namespace detail {

namespace has_get_pointer_
{

  struct any
  {
      template<class T> any(T const&);
  };

  struct no_overload_tag
  {};

  typedef char (&yes)[1];
  typedef char (&no)[2];

  no_overload_tag operator,(no_overload_tag, int);

//
// On compilers with ADL, we need these generic overloads in this
// namespace as well as in luaponte::. Otherwise get_pointer(any)
// will be found before them.
//
#ifndef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

  template<class T>
  T* get_pointer(T const volatile*);

  template<class T>
  T* get_pointer(std::auto_ptr<T> const&);

#endif

//
// On compilers that doesn't support ADL, the overload below has to
// live in luaponte::.
//
#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
}} // namespace detail::has_get_pointer_
#endif

detail::has_get_pointer_::no_overload_tag
  get_pointer(detail::has_get_pointer_::any);

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace detail { namespace has_get_pointer_
{
#endif

  template<class T>
  yes check(T const&);
  no check(no_overload_tag);

  template<class T>
  struct impl
  {
      static typename boost::add_reference<T>::type x;

      BOOST_STATIC_CONSTANT(bool,
          value = sizeof(has_get_pointer_::check( (get_pointer(x),0) )) == 1
      );

      typedef boost::mpl::bool_<value> type;
  };

} // namespace has_get_pointer_

template<class T>
struct has_get_pointer
  : has_get_pointer_::impl<T>::type
{};

} // namespace detail
} // namespace luaponte

#endif // LUAPONTE_DETAIL_HAS_GET_POINTER_HPP
