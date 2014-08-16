cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(cpp_trainning)

find_package(Boost "1.55.0" COMPONENTS program_options REQUIRED)

enable_testing()
include(CTest)
include(CPack)

include_directories(.)

add_executable(test_std_set.cpp test_std_set.cpp)

