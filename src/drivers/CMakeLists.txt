message(STATUS "in ${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB _srcs CONFIGURE_DEPENDS *.cpp *.cc)
foreach(_src ${_srcs})
    tcpp_sse(SRC ${_src} DEPS boost_wrapper root_wrapper defs TARGET_VAR _target)
endforeach()
