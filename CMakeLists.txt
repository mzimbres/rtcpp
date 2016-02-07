# Using a single CMakeLists.txt to avoid recursive makefiles.

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(rtcpp)

find_package(Boost "1.57.0" COMPONENTS container)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
 set(GNU_FOUND true)
endif()

configure_file(
  ${PROJECT_SOURCE_DIR}/src/config.h.in
  ${PROJECT_BINARY_DIR}/config.h
)

enable_testing()
include(CTest)
include(CPack)

include_directories(.)
include_directories(${PROJECT_BINARY_DIR})

install( DIRECTORY ${PROJECT_SOURCE_DIR}/rtcpp
         DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

add_executable(bench_set src/benchmarks/bench_set.cpp)
add_executable(bench_list src/benchmarks/bench_list.cpp)
add_executable(test_set src/tests/test_set.cpp)
add_executable(test_flist src/tests/test_flist.cpp)
add_executable(test_align src/tests/test_align.cpp)
add_executable(test_reverse src/tests/test_reverse.cpp)
add_executable(test_alloc src/tests/test_alloc.cpp)
add_executable(test_matrix src/tests/test_matrix.cpp)
add_executable(test_comp_counting_sort src/tests/test_comp_counting_sort.cpp)
add_executable(test_dist_counting_sort src/tests/test_dist_counting_sort.cpp)
add_executable(test_find_intrusive src/tests/test_find_intrusive.cpp)
add_executable(alloc_example_list src/examples/alloc_example_list.cpp)
add_executable(alloc_example_set src/examples/alloc_example_set.cpp)
add_executable(alloc_example_many src/examples/alloc_example_many.cpp)
add_executable(alloc_example_scoped src/examples/alloc_example_scoped.cpp)
add_executable(matrix_example1 src/examples/matrix_example1.cpp)
add_executable(bench_list_frag src/benchmarks/bench_list_frag.cpp)
add_executable(bench_find src/benchmarks/bench_find.cpp)

if (Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIR})
  target_link_libraries(bench_set ${Boost_LIBRARIES})
endif()

add_test(NAME test_set COMMAND test_set)
add_test(NAME test_reverse COMMAND test_reverse)
add_test(NAME test_align COMMAND test_align)
add_test(NAME test_alloc COMMAND test_alloc)
add_test(NAME test_matrix COMMAND test_matrix)
add_test(NAME test_comp_counting_sort COMMAND test_comp_counting_sort)
add_test(NAME test_dist_counting_sort COMMAND test_dist_counting_sort)
add_test(NAME test_find_intrusive COMMAND test_find_intrusive)
add_test(NAME alloc_example_list COMMAND alloc_example_list)
add_test(NAME alloc_example_set COMMAND alloc_example_set)
add_test(NAME alloc_example_many COMMAND alloc_example_many)
add_test(NAME alloc_example_scoped COMMAND alloc_example_scoped)
add_test(NAME matrix_example COMMAND matrix_example1)

