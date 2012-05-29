// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define LUAPONTE_BUILDING

#include <luaponte/detail/link_compatibility.hpp>

namespace luaponte {
namespace detail {

#ifdef LUAPONTE_NOT_THREADSAFE
void not_threadsafe_defined_conflict() {}
#else
void not_threadsafe_not_defined_conflict() {}
#endif

#ifdef LUAPONTE_NO_ERROR_CHECKING
void no_error_checking_defined_conflict() {}
#else
void no_error_checking_not_defined_conflict() {}
#endif

} // namespace detail
} // namespace luaponte
