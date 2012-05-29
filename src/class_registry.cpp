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
#include <luaponte/detail/class_registry.hpp>
#include <luaponte/detail/class_rep.hpp>
#include <luaponte/detail/operator_id.hpp>

namespace luaponte {
namespace detail {

LUAPONTE_API void push_instance_metatable(lua_State* L);

namespace {

int create_cpp_class_metatable(lua_State* L)
{
    lua_newtable(L);

    // mark the table with our (hopefully) unique tag
    // that says that the user data that has this
    // metatable is a class_rep
    lua_pushstring(L, "__luabind_classrep");
    lua_pushboolean(L, 1);
    lua_rawset(L, -3);

    lua_pushstring(L, "__gc");
    lua_pushcclosure(
        L
      , &garbage_collector_s<
            detail::class_rep
        >::apply
        , 0);

    lua_rawset(L, -3);

    lua_pushstring(L, "__call");
    lua_pushcclosure(L, &class_rep::constructor_dispatcher, 0);
    lua_rawset(L, -3);

    lua_pushstring(L, "__index");
    lua_pushcclosure(L, &class_rep::static_class_gettable, 0);
    lua_rawset(L, -3);

    lua_pushstring(L, "__newindex");
    lua_pushcclosure(L, &class_rep::lua_settable_dispatcher, 0);
    lua_rawset(L, -3);

    return luaL_ref(L, LUA_REGISTRYINDEX);
}

int create_lua_class_metatable(lua_State* L)
{
    lua_newtable(L);

    lua_pushstring(L, "__luabind_classrep");
    lua_pushboolean(L, 1);
    lua_rawset(L, -3);

    lua_pushstring(L, "__gc");
    lua_pushcclosure(
        L
      , &detail::garbage_collector_s<
            detail::class_rep
        >::apply
        , 0);

    lua_rawset(L, -3);

    lua_pushstring(L, "__newindex");
    lua_pushcclosure(L, &class_rep::lua_settable_dispatcher, 0);
    lua_rawset(L, -3);

    lua_pushstring(L, "__call");
    lua_pushcclosure(L, &class_rep::constructor_dispatcher, 0);
    lua_rawset(L, -3);

    lua_pushstring(L, "__index");
    lua_pushcclosure(L, &class_rep::static_class_gettable, 0);
    lua_rawset(L, -3);

    return luaL_ref(L, LUA_REGISTRYINDEX);
}

} // namespace

class class_rep;

class_registry::class_registry(lua_State* L)
    : m_cpp_class_metatable(create_cpp_class_metatable(L))
    , m_lua_class_metatable(create_lua_class_metatable(L))
{
    push_instance_metatable(L);
    m_instance_metatable = luaL_ref(L, LUA_REGISTRYINDEX);
}

class_registry* class_registry::get_registry(lua_State* L)
{

#ifdef LUAPONTE_NOT_THREADSAFE

    // if we don't have to be thread safe, we can keep a
    // chache of the class_registry pointer without the
    // need of a mutex
    static lua_State* cache_key = 0;
    static class_registry* registry_cache = 0;
    if (cache_key == L) return registry_cache;

#endif

    lua_pushstring(L, "__luabind_classes");
    lua_gettable(L, LUA_REGISTRYINDEX);
    class_registry* p = static_cast<class_registry*>(lua_touserdata(L, -1));
    lua_pop(L, 1);

#ifdef LUAPONTE_NOT_THREADSAFE

    cache_key = L;
    registry_cache = p;

#endif

    return p;
}

void class_registry::add_class(type_id const& info, class_rep* crep)
{
    // class is already registered
    assert((m_classes.find(info) == m_classes.end())
        && "you are trying to register a class twice");
    m_classes[info] = crep;
}

class_rep* class_registry::find_class(type_id const& info) const
{
    std::map<type_id, class_rep*>::const_iterator i(
        m_classes.find(info));

    if (i == m_classes.end()) return 0; // the type is not registered
    return i->second;
}

} // namespace detail
} // namespace luaponte
