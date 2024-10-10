find_package(Boost REQUIRED CONFIG)

tcpp_target_name(_target)

tcpp_debug_var(Boost_INCLUDE_DIRS)
tcpp_debug_var(Boost_LIBRARIES)

add_library(${_target} INTERFACE)
target_link_libraries(${_target} INTERFACE ${Boost_LIBRARIES})
target_include_directories(${_target} INTERFACE ${Boost_LIBRARIES})
