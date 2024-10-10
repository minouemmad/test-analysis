tcpp_target_name(_target)
add_library(${_target} INTERFACE)
target_include_directories(${_target} INTERFACE ${CMAKE_SOURCE_DIR}/src ${CMAKE_BINARY_DIR}/src)
