add_library(luabind
  class.cpp
  class_info.cpp
  class_registry.cpp
  class_rep.cpp
  create_class.cpp
  error.cpp
  exception_handler.cpp
  function.cpp
  inheritance.cpp
  link_compatibility.cpp
  object_rep.cpp
  open.cpp
  pcall.cpp
  scope.cpp
  stack_content_by_name.cpp
  weak_ref.cpp
  wrapper_base.cpp
)

# Suppress GCC warnings about deprecated std::auto_ptr
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set_target_properties(luabind PROPERTIES COMPILE_FLAGS "-Wno-deprecated")
endif()
