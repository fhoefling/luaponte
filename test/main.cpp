// Luaponte library

// Copyright (c) 2011-2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <cstring>

extern "C"
{
    #include "lauxlib.h"
    #include "lualib.h"
}

#include <luaponte/open.hpp>
#include "test.hpp"

extern "C" struct lua_State;

void test_main(lua_State*);

struct lua_state
{
    lua_state();
    ~lua_state();

    operator lua_State*() const;
    void check() const;

private:
    lua_State* m_state;
    int m_top;
};

lua_state::lua_state()
    : m_state(luaL_newstate())
{
    luaopen_base(m_state);
#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
     // lua 5.1 or newer
     luaL_openlibs(m_state);
#else
     // lua 5.0.2 or older
    lua_baselibopen(m_state);
#endif
    m_top = lua_gettop(m_state);
    luaponte::open(m_state);
}

lua_state::~lua_state()
{
    lua_close(m_state);
}

void lua_state::check() const
{
    TEST_CHECK(lua_gettop(m_state) == m_top);
}

lua_state::operator lua_State*() const
{
    return m_state;
}

int pcall_handler(lua_State* L)
{
    return 1;
}

void dostring(lua_State* state, char const* str)
{
    lua_pushcclosure(state, &pcall_handler, 0);

    if (luaL_loadbuffer(state, str, std::strlen(str), str))
    {
        std::string err(lua_tostring(state, -1));
        lua_pop(state, 2);
        throw err;
    }

    if (lua_pcall(state, 0, 0, -2))
    {
        std::string err(lua_tostring(state, -1));
        lua_pop(state, 2);
        throw err;
    }

    lua_pop(state, 1);
}

bool tests_failure = false;

void report_failure(char const* err, char const* file, int line)
{
    std::cerr << file << ":" << line << "\"" << err << "\"\n";
    tests_failure = true;
}

int main()
{
    lua_state L;
    try
    {
        test_main(L);
        L.check();
        return tests_failure ? 1 : 0;
    }
    catch (luaponte::error const& e)
    {
        std::cerr << "Terminated with exception: \"" << e.what() << "\"\n"
            << lua_tostring(e.state(), -1) << "\n";
        return 1;
    }
    catch (std::exception const& e)
    {
        std::cerr << "Terminated with exception: \"" << e.what() << "\"\n";
        return 1;
    }
    catch (...)
    {
        std::cerr << "Terminated with unknown exception\n";
        return 1;
    }
}
