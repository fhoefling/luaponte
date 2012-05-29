// Luaponte library

// Copyright (c) 2011-2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define LUAPONTE_BUILDING

#include <luaponte/lua_include.hpp>

#include <luaponte/luaponte.hpp>

#if LUA_VERSION_NUM < 502
# define lua_compare(L, index1, index2, fn) fn(L, index1, index2)
# define LUA_OPEQ lua_equal
# define lua_rawlen lua_objlen
#endif

namespace luaponte {
namespace detail {

namespace {

// expects two tables on the lua stack:
// 1: destination
// 2: source
void copy_member_table(lua_State* L)
{
    lua_pushnil(L);

    while (lua_next(L, -2))
    {
        lua_pushstring(L, "__init");
        if (lua_compare(L, -1, -3, LUA_OPEQ))
        {
            lua_pop(L, 2);
            continue;
        }
        else lua_pop(L, 1); // __init string

        lua_pushstring(L, "__finalize");
        if (lua_compare(L, -1, -3, LUA_OPEQ))
        {
            lua_pop(L, 2);
            continue;
        }
        else lua_pop(L, 1); // __finalize string

        lua_pushvalue(L, -2); // copy key
        lua_insert(L, -2);
        lua_settable(L, -5);
    }
}

} // namespace

int create_class::stage2(lua_State* L)
{
    class_rep* crep = static_cast<class_rep*>(lua_touserdata(L, lua_upvalueindex(1)));
    assert((crep != 0) && "internal error, please report");
    assert((is_class_rep(L, lua_upvalueindex(1))) && "internal error, please report");

#ifndef LUAPONTE_NO_ERROR_CHECKING

    if (!is_class_rep(L, 1))
    {
        lua_pushstring(L, "expected class to derive from or a newline");
        lua_error(L);
    }

#endif

    class_rep* base = static_cast<class_rep*>(lua_touserdata(L, 1));
    class_rep::base_info binfo;

    binfo.pointer_offset = 0;
    binfo.base = base;
    crep->add_base_class(binfo);

    // copy base class members

    crep->get_table(L);
    base->get_table(L);
    copy_member_table(L);

    crep->get_default_table(L);
    base->get_default_table(L);
    copy_member_table(L);

    crep->set_type(base->type());

    return 0;
}

int create_class::stage1(lua_State* L)
{

#ifndef LUAPONTE_NO_ERROR_CHECKING

    if (lua_gettop(L) != 1 || lua_type(L, 1) != LUA_TSTRING || lua_isnumber(L, 1))
    {
        lua_pushstring(L, "invalid construct, expected class name");
        lua_error(L);
    }

    if (std::strlen(lua_tostring(L, 1)) != lua_rawlen(L, 1))
    {
        lua_pushstring(L, "luabind does not support class names with extra nulls");
        lua_error(L);
    }

#endif

    const char* name = lua_tostring(L, 1);

    void* c = lua_newuserdata(L, sizeof(class_rep));
    new(c) class_rep(L, name);

    // make the class globally available
    lua_pushvalue(L, -1);
    lua_setglobal(L, name);

    // also add it to the closure as return value
    lua_pushcclosure(L, &stage2, 1);

    return 1;
}

} // namespace detail
} // namespace luaponte
