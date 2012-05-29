// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_TEST_HPP
#define LUAPONTE_TEST_HPP

#include <boost/preprocessor/cat.hpp>
#include <luaponte/error.hpp>

extern "C"
{
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

void report_failure(char const* str, char const* file, int line);

#if defined(_MSC_VER)
#define COUNTER_GUARD(x)
#else
#define COUNTER_GUARD(type) \
    struct BOOST_PP_CAT(type, _counter_guard) \
    { \
        ~BOOST_PP_CAT(type, _counter_guard()) \
        { \
            TEST_CHECK(counted_type<type>::count == 0); \
        } \
    } BOOST_PP_CAT(type, _guard)
#endif

#define TEST_REPORT_AUX(x, line, file) \
    report_failure(x, line, file)

#define TEST_CHECK(x) \
    if (!(x)) \
        TEST_REPORT_AUX("TEST_CHECK failed: \"" #x "\"", __FILE__, __LINE__)

#define TEST_ERROR(x) \
    TEST_REPORT_AUX((std::string("ERROR: \"") + x + "\"").c_str(), __FILE__, __LINE__)

#define TEST_NOTHROW(x) \
    try \
    { \
        x; \
    } \
    catch (...) \
    { \
        TEST_ERROR("Exception thrown: " #x); \
    }

void dostring(lua_State* L, char const* str);

template<class T>
struct counted_type
{
    static int count;

    counted_type()
    {
        ++count;
    }

    counted_type(counted_type const&)
    {
        ++count;
    }

    ~counted_type()
    {
        TEST_CHECK(--count >= 0);
    }
};

template<class T>
int counted_type<T>::count = 0;

#define DOSTRING_EXPECTED(state_, str, expected) \
{                                               \
    try                                         \
    {                                           \
        dostring(state_, str);                  \
    }                                           \
    catch (luaponte::error const& e)             \
    {                                           \
        using namespace std;                    \
        if (std::strcmp(                        \
            lua_tostring(e.state(), -1)         \
          , (char const*)expected))             \
        {                                       \
            TEST_ERROR(lua_tostring(e.state(), -1)); \
            lua_pop(L, 1);                      \
        }                                       \
    }                                           \
    catch (std::string const& s)                \
    {                                           \
        if (s != expected)                      \
            TEST_ERROR(s.c_str());              \
    }                                           \
}

#define DOSTRING(state_, str)                   \
{                                               \
    try                                         \
    {                                           \
        dostring(state_, str);                  \
    }                                           \
    catch (luaponte::error const& e)             \
    {                                           \
        TEST_ERROR(lua_tostring(e.state(), -1)); \
            lua_pop(L, 1);                      \
    }                                           \
    catch (std::string const& s)                \
    {                                           \
        TEST_ERROR(s.c_str());                  \
    }                                           \
}

#endif // LUAPONTE_TEST_HPP
