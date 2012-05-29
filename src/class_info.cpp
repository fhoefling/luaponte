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
#include <luaponte/class_info.hpp>
#include <luaponte/detail/class_registry.hpp>

namespace luaponte {

LUAPONTE_API class_info get_class_info(argument const& o)
{
    lua_State* L = o.interpreter();

    o.push(L);
    detail::object_rep* obj = detail::get_instance(L, -1);

    if (!obj)
    {
        class_info result;
        result.name = lua_typename(L, lua_type(L, -1));
        lua_pop(L, 1);
        result.methods = newtable(L);
        result.attributes = newtable(L);
        return result;
    }

    lua_pop(L, 1);

    obj->crep()->get_table(L);
    object table(from_stack(L, -1));
    lua_pop(L, 1);

    class_info result;
    result.name = detail::get_class_name(L, obj->crep()->type());
    result.methods = newtable(L);
    result.attributes = newtable(L);

    std::size_t index = 1;

    for (iterator i(table), e; i != e; ++i)
    {
        if (type(*i) != LUA_TFUNCTION)
            continue;

        // We have to create a temporary `object` here, otherwise the proxy
        // returned by operator->() will mess up the stack. This is a known
        // problem that probably doesn't show up in real code very often.
        object member(*i);
        member.push(L);
        detail::stack_pop pop(L, 1);

        if (lua_tocfunction(L, -1) == &detail::property_tag)
        {
            result.attributes[index++] = i.key();
        }
        else
        {
            result.methods[i.key()] = *i;
        }
    }

    return result;
}

LUAPONTE_API object get_class_names(lua_State* L)
{
    detail::class_registry* reg = detail::class_registry::get_registry(L);

    std::map<type_id, detail::class_rep*> const& classes = reg->get_classes();

    object result = newtable(L);
    std::size_t index = 1;

    for (std::map<type_id, detail::class_rep*>::const_iterator iter = classes.begin();
        iter != classes.end(); ++iter)
    {
        result[index++] = iter->second->name();
    }

    return result;
}

LUAPONTE_API void bind_class_info(lua_State* L)
{
    module(L)
    [
        class_<class_info>("class_info_data")
            .def_readonly("name", &class_info::name)
            .def_readonly("methods", &class_info::methods)
            .def_readonly("attributes", &class_info::attributes),

        def("class_info", &get_class_info),
        def("class_names", &get_class_names)
    ];
}

} // namespace luaponte
