# Locate Lua library
# This module defines
#  LUA_FOUND
#  LUA_LIBRARIES
#  LUA_INCLUDE_DIR
#

#=============================================================================
# Copyright 2007-2009 Kitware, Inc.
# Copyright 2011-2012 Peter Colberg
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

find_path(LUA_INCLUDE_DIR lua.h
  HINTS $ENV{LUA_DIR}
  PATH_SUFFIXES lua52 lua5.2 lua51 lua5.1 lua
)

find_library(LUA_LIBRARY
  NAMES lua lua52 lua5.2 lua-5.2 lua51 lua5.1 lua-5.1
)

if(LUA_LIBRARY)
  # include the math library for Unix
  if(UNIX AND NOT APPLE)
    find_library(LUA_MATH_LIBRARY m)
    set(LUA_LIBRARIES "${LUA_LIBRARY};${LUA_MATH_LIBRARY}" CACHE STRING "Lua Libraries")
  # For Windows and Mac, don't need to explicitly include the math library
  else(UNIX AND NOT APPLE)
    set(LUA_LIBRARIES "${LUA_LIBRARY}" CACHE STRING "Lua Libraries")
  endif(UNIX AND NOT APPLE)
endif(LUA_LIBRARY)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LUA_FOUND to TRUE if
# all listed variables are TRUE
find_package_handle_standard_args(Lua DEFAULT_MSG LUA_LIBRARIES LUA_INCLUDE_DIR)

mark_as_advanced(
  LUA_INCLUDE_DIR
  LUA_LIBRARIES
  LUA_LIBRARY
  LUA_MATH_LIBRARY
)
