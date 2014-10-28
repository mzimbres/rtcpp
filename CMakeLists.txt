cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(rtcpp)

find_package(Boost "1.55.0" COMPONENTS timer system program_options REQUIRED)

enable_testing()
include(CTest)
include(CPack)

include_directories(.)

add_executable(test_bst tests/test_bst.cpp)
add_executable(test_bst_iterator tests/test_bst_iterator.cpp)
add_executable(test_flist tests/test_flist.cpp)
add_executable(test_sort tests/sort.cpp)
add_executable(test_comp_count_sort tests/test_comp_count_sort.cpp)

if (Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIR})
  add_executable(bench_bst benchmarks/bench_bst.cpp)
  target_link_libraries(bench_bst ${Boost_LIBRARIES})
endif()

add_test(NAME test_comp_count_sort COMMAND test_comp_count_sort)
add_test(NAME test_bst COMMAND test_bst)
add_test(NAME test_bst_iterator COMMAND test_bst_iterator)

