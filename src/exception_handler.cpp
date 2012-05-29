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
#include <luaponte/exception_handler.hpp>
#include <luaponte/error.hpp>
#include <stdexcept>

#ifndef LUAPONTE_NO_EXCEPTIONS

namespace luaponte {
namespace detail {

namespace {

exception_handler_base* handler_chain = 0;

void push_exception_string(lua_State* L, char const* exception, char const* what)
{
    lua_pushstring(L, exception);
    lua_pushstring(L, ": '");
    lua_pushstring(L, what);
    lua_pushstring(L, "'");
    lua_concat(L, 4);
}

} // namespace

void exception_handler_base::try_next(lua_State* L) const
{
    if (next)
        next->handle(L);
    else
        throw;
}

LUAPONTE_API void handle_exception_aux(lua_State* L)
{
    try
    {
        if (handler_chain)
            handler_chain->handle(L);
        else
            throw;
    }
    catch (error const&)
    {}
    catch (std::logic_error const& e)
    {
        push_exception_string(L, "std::logic_error", e.what());
    }
    catch (std::runtime_error const& e)
    {
        push_exception_string(L, "std::runtime_error", e.what());
    }
    catch (std::exception const& e)
    {
        push_exception_string(L, "std::exception", e.what());
    }
    catch (char const* str)
    {
        push_exception_string(L, "c-string", str);
    }
    catch (...)
    {
        lua_pushstring(L, "Unknown C++ exception");
    }
}

LUAPONTE_API void register_exception_handler(exception_handler_base* handler)
{
    if (!handler_chain) handler_chain = handler;
    else
    {
        exception_handler_base* p = handler_chain;

        for (; p->next; p = p->next);

        handler->next = 0;
        p->next = handler;
    }
}

} // namespace detail
} // namespace luaponte

#endif // LUAPONTE_NO_EXCEPTIONS
