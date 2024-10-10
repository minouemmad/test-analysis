tcpp_target_name(_target)

file(GLOB _hdrs CONFIGURE_DEPENDS *.h)
add_library(${_target} INTERFACE ${_hdrs})
target_link_libraries(${_target} INTERFACE cpp17 base_include)
target_include_directories(${_target} INTERFACE ./)
