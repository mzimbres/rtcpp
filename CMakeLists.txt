cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(rtcpp)

find_package(Boost "1.55.0" COMPONENTS timer system program_options OPTIONAL)

enable_testing()
include(CTest)
include(CPack)

include_directories(.)

if (UNIX)
  add_executable(bench_set benchmarks/bench_set.cpp)
  add_executable(test_set tests/test_set.cpp)
  add_test(NAME test_set COMMAND test_set)
endif (UNIX)

add_executable(test_flist tests/test_flist.cpp)
add_executable(test_align tests/test_align.cpp)
add_executable(test_reverse tests/test_reverse.cpp)
add_executable(test_alloc tests/test_alloc.cpp)
add_executable(example1 examples/example1.cpp)
add_executable(example2 examples/example2.cpp)
add_executable(bench_map benchmarks/bench_map.cpp)

#if (Boost_FOUND)
#  include_directories(${Boost_INCLUDE_DIR})
#  target_link_libraries(bench_set ${Boost_LIBRARIES})
#endif()

add_test(NAME test_reverse COMMAND test_reverse)
add_test(NAME test_align COMMAND test_align)
add_test(NAME test_alloc COMMAND test_alloc)
add_test(NAME example1 COMMAND example1)
add_test(NAME example2 COMMAND example2)

