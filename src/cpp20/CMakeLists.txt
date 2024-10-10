tcpp_target_name(_target)
add_library(${_target} INTERFACE)
target_compile_features(${_target} INTERFACE cxx_std_20)
