cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(rtcpp)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(GNU_FOUND true)
endif()

configure_file(
  ${PROJECT_SOURCE_DIR}/config.h.in
  ${PROJECT_BINARY_DIR}/config.h
)

enable_testing()
include(CTest)
include(CPack)

include_directories(..)
include_directories(${PROJECT_BINARY_DIR})

install( DIRECTORY ${PROJECT_SOURCE_DIR}/include/rtcpp
         DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

add_executable(ex_matrix          ${PROJECT_SOURCE_DIR}/ex_matrix.cpp)
add_executable(test_align         ${PROJECT_SOURCE_DIR}/test_align.cpp)
add_executable(test_matrix        ${PROJECT_SOURCE_DIR}/test_matrix.cpp)
add_executable(test_sort          ${PROJECT_SOURCE_DIR}/test_sort.cpp)
add_executable(test_algorithm     ${PROJECT_SOURCE_DIR}/test_algorithm.cpp)
add_executable(test_tree          ${PROJECT_SOURCE_DIR}/test_tree.cpp)
add_executable(test_combinatorics ${PROJECT_SOURCE_DIR}/test_combinatorics.cpp)

add_executable(interview_bit_img        ${PROJECT_SOURCE_DIR}/interview_bit_img.cpp)
add_executable(interview_bits           ${PROJECT_SOURCE_DIR}/interview_bits.cpp)
add_executable(interview_interval_map   ${PROJECT_SOURCE_DIR}/interview_interval_map.cpp)
add_executable(interview_logging_thread ${PROJECT_SOURCE_DIR}/interview_logging_thread.cpp)
add_executable(interview_str_arithmetic ${PROJECT_SOURCE_DIR}/interview_str_arithmetic.cpp)

add_executable(tool_book       ${PROJECT_SOURCE_DIR}/tool_book.cpp)
add_executable(tool_bench_sort ${PROJECT_SOURCE_DIR}/tool_bench_sort.cpp)

add_test(NAME ex_matrix          COMMAND ex_matrix)
add_test(NAME test_sort          COMMAND test_sort)
add_test(NAME test_align         COMMAND test_align)
add_test(NAME test_tree          COMMAND test_tree)
add_test(NAME test_combinatorics COMMAND test_combinatorics)
add_test(NAME test_matrix        COMMAND test_matrix)
add_test(NAME test_algorithm     COMMAND test_algorithm)

add_test(NAME interview_bit_img        COMMAND interview_bit_img)
add_test(NAME interview_bits           COMMAND interview_bits)
add_test(NAME interview_interval_map   COMMAND interview_interval_map)
add_test(NAME interview_logging_thread COMMAND interview_logging_thread)
add_test(NAME interview_str_arithmetic COMMAND interview_str_arithmetic)

