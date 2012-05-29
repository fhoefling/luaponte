cmake_minimum_required(VERSION 2.8)

set(CMAKE_USER_MAKE_RULES_OVERRIDE "cmake/platform.cmake")

project(Luaponte CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(Boost 1.49.0 QUIET REQUIRED)
find_package(Lua QUIET REQUIRED)

include_directories(${Boost_INCLUDE_DIR})
include_directories(${LUA_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR})

enable_testing()
include(CTest)

add_subdirectory(src)
add_subdirectory(test)
