set(CMAKE_BUILD_TYPE_INIT "Release")

if(DEFINED CMAKE_CXX_COMPILER_ID)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

    set(CMAKE_CXX_FLAGS_INIT "-fPIC -Wall -std=c++11 -pedantic")
    set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O3 -DNDEBUG -DBOOST_DISABLE_ASSERTS -fvisibility=hidden")

  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

    set(CMAKE_CXX_FLAGS_INIT "-fPIC -Wall -std=c++11 -pedantic")
    set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O3 -DNDEBUG -DBOOST_DISABLE_ASSERTS -fvisibility=hidden")

  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")

    set(CMAKE_CXX_FLAGS_INIT "-fPIC -Wall")
    set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O3 -DNDEBUG -DBOOST_DISABLE_ASSERTS -fvisibility=hidden")

  else()
    message(WARNING "Unsupported CXX compiler: ${CMAKE_CXX_COMPILER_ID}")
  endif()
endif()
