// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define LUAPONTE_BUILDING

#include <algorithm>

#include <luaponte/lua_include.hpp>

#include <luaponte/config.hpp>
#include <luaponte/weak_ref.hpp>
#include <cassert>

namespace luaponte {

namespace {

int weak_table_tag;

} // namespace

LUAPONTE_API void get_weak_table(lua_State* L)
{
    lua_pushlightuserdata(L, &weak_table_tag);
    lua_rawget(L, LUA_REGISTRYINDEX);

    if (lua_isnil(L, -1))
    {
        lua_pop(L, 1);
        lua_newtable(L);
        // metatable
        lua_newtable(L);
        lua_pushliteral(L, "__mode");
        lua_pushliteral(L, "v");
        lua_rawset(L, -3);
        // set metatable
        lua_setmetatable(L, -2);

        lua_pushlightuserdata(L, &weak_table_tag);
        lua_pushvalue(L, -2);
        lua_rawset(L, LUA_REGISTRYINDEX);
    }
}

struct weak_ref::impl
{
    impl(lua_State* main, lua_State* s, int index)
        : count(0)
        , state(main)
        , ref(0)
    {
        get_weak_table(s);
        lua_pushvalue(s, index);
        ref = luaL_ref(s, -2);
        lua_pop(s, 1);
    }

    ~impl()
    {
        get_weak_table(state);
        luaL_unref(state, -1, ref);
        lua_pop(state, 1);
    }

    int count;
    lua_State* state;
    int ref;
};

weak_ref::weak_ref()
    : m_impl(0)
{
}

weak_ref::weak_ref(lua_State* main, lua_State* L, int index)
    : m_impl(new impl(main, L, index))
{
    m_impl->count = 1;
}

weak_ref::weak_ref(weak_ref const& other)
    : m_impl(other.m_impl)
{
    if (m_impl) ++m_impl->count;
}

weak_ref::~weak_ref()
{
    if (m_impl && --m_impl->count == 0)
    {
        delete m_impl;
    }
}

weak_ref& weak_ref::operator=(weak_ref const& other)
{
    weak_ref(other).swap(*this);
    return *this;
}

void weak_ref::swap(weak_ref& other)
{
    std::swap(m_impl, other.m_impl);
}

int weak_ref::id() const
{
    assert(m_impl);
    return m_impl->ref;
}

// L may not be the same pointer as
// was used when creating this reference
// since it may be a thread that shares
// the same globals table.
void weak_ref::get(lua_State* L) const
{
    assert(m_impl);
    assert(L);
    get_weak_table(L);
    lua_rawgeti(L, -1, m_impl->ref);
    lua_remove(L, -2);
}

lua_State* weak_ref::state() const
{
    assert(m_impl);
    return m_impl->state;
}

} // namespace luaponte
