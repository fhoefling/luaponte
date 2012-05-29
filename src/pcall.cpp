// Luaponte library

// Copyright (c) 2011-2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define LUAPONTE_BUILDING

#include <luaponte/detail/pcall.hpp>
#include <luaponte/error.hpp>
#include <luaponte/lua_include.hpp>

namespace luaponte {
namespace detail {

int pcall(lua_State *L, int nargs, int nresults)
{
    pcall_callback_fun e = get_pcall_callback();
    int en = 0;
    if ( e )
    {
        int base = lua_gettop(L) - nargs;
        lua_pushcfunction(L, e);
        lua_insert(L, base);  // push pcall_callback under chunk and args
        en = base;
    }
    int result = lua_pcall(L, nargs, nresults, en);
    if ( en )
        lua_remove(L, en);  // remove pcall_callback
    return result;
}

int resume_impl(lua_State *L, int nargs, int)
{
#if LUA_VERSION_NUM >= 502
    int res = lua_resume(L, NULL, nargs);
#else
    int res = lua_resume(L, nargs);
#endif
    // Lua 5.1 added  LUA_YIELD as a possible return value,
    // this was causing crashes, because the caller expects 0 on success.
    return (res == LUA_YIELD) ? 0 : res;
}

} // namespace detail
} // namespace luaponte
