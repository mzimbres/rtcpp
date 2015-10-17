cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(rtcpp)

find_package(Boost "1.57.0" COMPONENTS container)

configure_file(
  ${PROJECT_SOURCE_DIR}/config.h.in
  ${PROJECT_BINARY_DIR}/config.h
)

enable_testing()
include(CTest)
include(CPack)

include_directories(.)
include_directories(${PROJECT_BINARY_DIR})

if (UNIX)
  add_executable(bench_set benchmarks/bench_set.cpp)
  add_executable(bench_list benchmarks/bench_list.cpp)
  add_executable(test_set tests/test_set.cpp)
  add_test(NAME test_set COMMAND test_set)
endif (UNIX)

add_executable(test_flist tests/test_flist.cpp)
add_executable(test_align tests/test_align.cpp)
add_executable(test_reverse tests/test_reverse.cpp)
add_executable(test_alloc tests/test_alloc.cpp)
add_executable(test_matrix tests/test_matrix.cpp)
add_executable(alloc_example1 examples/alloc_example1.cpp)
add_executable(alloc_example2 examples/alloc_example2.cpp)
add_executable(alloc_example3 examples/alloc_example3.cpp)
add_executable(matrix_example1 examples/matrix_example1.cpp)
add_executable(bench_list_frag benchmarks/bench_list_frag.cpp)

if (Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIR})
  target_link_libraries(bench_set ${Boost_LIBRARIES})
endif()

add_test(NAME test_reverse COMMAND test_reverse)
add_test(NAME test_align COMMAND test_align)
add_test(NAME test_alloc COMMAND test_alloc)
add_test(NAME test_matrix COMMAND test_matrix)
add_test(NAME alloc_example1 COMMAND alloc_example1)
add_test(NAME alloc_example2 COMMAND alloc_example2)
add_test(NAME alloc_example3 COMMAND alloc_example3)
add_test(NAME matrix_example COMMAND matrix_example1)

