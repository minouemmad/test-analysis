tcpp_target_name(_target)

tcpp_debug_var(ROOT_LIBRARIES)
tcpp_debug_var(ROOT_DEFINITIONS)
tcpp_debug_var(ROOT_INCLUDE_DIRS)
tcpp_debug_var(ROOT_CXX_FLAGS)

add_library(${_target} INTERFACE)
target_link_libraries(${_target} INTERFACE ${ROOT_LIBRARIES})
#target_compile_options(${_target} INTERAFCE ${ROOT_CXX_FLAGS})
target_include_directories(${_target} INTERFACE ${ROOT_INCLUDE_DIRS})
