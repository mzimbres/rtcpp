cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(cpp_trainning)

find_package(Boost "1.55.0" COMPONENTS program_options REQUIRED)

enable_testing()
include(CTest)
include(CPack)

include_directories(.)
include_directories(${Boost_INCLUDE_DIR})

add_executable(test_std_set test_std_set.cpp)
add_executable(test_flat_set test_flat_set.cpp)
add_executable(test_bst test_tree.cpp)
add_executable(test_flist test_flist.cpp)
add_executable(test_sort sort.cpp)

#target_link_libraries(test_nmea ${Boost_LIBRARIES})

