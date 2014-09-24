cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(cpp_trainning)

find_package(Boost "1.55.0" COMPONENTS program_options REQUIRED)

enable_testing()
include(CTest)
include(CPack)

include_directories(.)
include_directories(${Boost_INCLUDE_DIR})

add_executable(test_std_set tests/test_std_set.cpp)
add_executable(test_bst tests/test_bst.cpp)
add_executable(test_flist tests/test_flist.cpp)
add_executable(test_sort tests/sort.cpp)
add_executable(test_comp_count_sort tests/test_comp_count_sort.cpp)

if (Boost_FOUND)
  add_executable(test_flat_set tests/test_flat_set.cpp)
endif()

add_test(NAME test_comp_count_sort COMMAND test_comp_count_sort)

