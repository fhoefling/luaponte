// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define LUAPONTE_BUILDING

#include <luaponte/config.hpp>
#include <luaponte/lua_include.hpp>
#include <luaponte/function.hpp>
#include <luaponte/detail/object_rep.hpp>
#include <luaponte/detail/class_rep.hpp>
#include <luaponte/detail/stack_utils.hpp>

namespace luaponte {
namespace detail {

LUAPONTE_API void do_call_member_selection(lua_State* L, char const* name)
{
    object_rep* obj = static_cast<object_rep*>(lua_touserdata(L, -1));

    lua_pushstring(L, name);
    lua_gettable(L, -2);
    lua_replace(L, -2);

    if (!is_luabind_function(L, -1))
        return;

    // this (usually) means the function has not been
    // overridden by lua, call the default implementation
    lua_pop(L, 1);
    obj->crep()->get_default_table(L); // push the crep table
    lua_pushstring(L, name);
    lua_gettable(L, -2);
    lua_remove(L, -2); // remove the crep table
}

} // namespace detail
} // namespace luaponte
