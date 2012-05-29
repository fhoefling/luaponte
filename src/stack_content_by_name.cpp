// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define LUAPONTE_BUILDING

#include <luaponte/lua_include.hpp>

#include <luaponte/luaponte.hpp>

using namespace luaponte::detail;

std::string luaponte::detail::stack_content_by_name(lua_State* L, int start_index)
{
    std::string ret;
    int top = lua_gettop(L);
    for (int i = start_index; i <= top; ++i)
    {
        object_rep* obj = get_instance(L, i);
        class_rep* crep = is_class_rep(L, i)?(class_rep*)lua_touserdata(L, i):0;
        if (obj == 0 && crep == 0)
        {
            int type = lua_type(L, i);
            ret += lua_typename(L, type);
        }
        else if (obj)
        {
            if (obj->is_const()) ret += "const ";
            ret += obj->crep()->name();
        }
        else if (crep)
        {
            ret += "<";
            ret += crep->name();
            ret += ">";
        }
        if (i < top) ret += ", ";
    }
    return ret;
}
