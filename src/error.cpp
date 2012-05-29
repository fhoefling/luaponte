// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define LUAPONTE_BUILDING

#include <luaponte/error.hpp>

namespace luaponte {

    namespace
    {
        pcall_callback_fun pcall_callback = 0;
#ifdef LUAPONTE_NO_EXCEPTIONS
        error_callback_fun error_callback = 0;
        cast_failed_callback_fun cast_failed_callback = 0;
#endif
    }


#ifdef LUAPONTE_NO_EXCEPTIONS

    typedef void(*error_callback_fun)(lua_State*);
    typedef void(*cast_failed_callback_fun)(lua_State*, type_id const&);

    void set_error_callback(error_callback_fun e)
    {
        error_callback = e;
    }

    void set_cast_failed_callback(cast_failed_callback_fun c)
    {
        cast_failed_callback = c;
    }

    error_callback_fun get_error_callback()
    {
        return error_callback;
    }

    cast_failed_callback_fun get_cast_failed_callback()
    {
        return cast_failed_callback;
    }

#endif

    void set_pcall_callback(pcall_callback_fun e)
    {
        pcall_callback = e;
    }

    pcall_callback_fun get_pcall_callback()
    {
        return pcall_callback;
    }

}
