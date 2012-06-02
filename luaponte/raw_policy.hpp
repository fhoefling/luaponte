// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_RAW_POLICY_HPP
#define LUAPONTE_RAW_POLICY_HPP

#include <luaponte/config.hpp>
#include <luaponte/detail/policy.hpp>

namespace luaponte {
namespace detail {

struct raw_converter
{
    int consumed_args(...) const
    {
        return 0;
    }

    lua_State* apply(lua_State* L, by_pointer<lua_State>, int)
    {
        return L;
    }

    static int match(...)
    {
        return 0;
    }

    void converter_postcall(lua_State*, by_pointer<lua_State>, int) {}
};

template<int N>
struct raw_policy : conversion_policy<N, false>
{
    static void precall(lua_State*, const index_map&) {}
    static void postcall(lua_State*, const index_map&) {}

    template<class T, class Direction>
    struct apply
    {
        typedef raw_converter type;
    };
};

} // namespace detail

template<int N>
detail::policy_cons<
    detail::raw_policy<N>
  , detail::null_type
>
inline raw(LUAPONTE_PLACEHOLDER_ARG(N))
{
    return detail::policy_cons<
        detail::raw_policy<N>
      , detail::null_type
    >();
}

} // namespace luaponte

#endif // LUAPONTE_RAW_POLICY_HPP
