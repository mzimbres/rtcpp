#include <vector>
#include <limits>
#include <iostream>
#include <iterator>
#include <exception>
#include <algorithm>

#include "rtcpp.hpp"
#include "test.hpp"

namespace
{
  const auto sort_size = 5000;
}

#define TEST_SORT(name)                                \
void test_##name()                                     \
{                                                      \
  auto data =                                          \
    rt::make_rand_data(sort_size, 1,                   \
       std::numeric_limits<int>::max());               \
                                                       \
  rt::name(std::begin(data), std::end(data));          \
                                                       \
  const auto b =                                       \
     std::is_sorted(std::begin(data), std::end(data)); \
  if (!b)                                              \
    throw std::runtime_error(#name);                   \
}

// Code fragment useful to debug.
//void test_sort()
//{
//  std::vector<int> data {4, 7, 2, 1, 8, 9, 4, 5, 2, 1, 8};
//  rt::print(data);
//
//  rt::straight_selection(std::begin(data), std::end(data));
//
//  const auto b = std::is_sorted(std::begin(data), std::end(data));
//  if (!b)
//    throw std::runtime_error("Insertion sort error.");
//
//  rt::print(data);
//}

TEST_SORT(bubble_sort);
TEST_SORT(comparison_counting_sort);
TEST_SORT(inplace_comparison_counting_sort);
TEST_SORT(straight_insertion);
TEST_SORT(straight_selection);
TEST_SORT(tree_insertion_sort);
TEST_SORT(binary_insertion);

void test_merge_sort()
{
  //auto data = rt::make_rand_data(20, 1, 100, 1);
  std::vector<int> data1 {1, 2, 3, 5, 7, 9};
  std::vector<int> data2 {3, 4, 5, 6, 8, 10};
  rt::print(data1);
  rt::print(data2);

  std::vector<int> vec;
  rt::merge( std::begin(data1), std::end(data1)
           , std::begin(data2), std::end(data2)
           , std::back_inserter(vec));

  const auto b = std::is_sorted(std::begin(vec), std::end(vec));
  if (!b)
    throw std::runtime_error("Merge sort error.");

  rt::print(vec);
}

void test_dist_count_sort()
{
  auto N = 200000;
  auto A = -20;
  auto B = 200;
  auto data = rt::make_rand_data(N ,A ,B);

  rt::dist_counting_sort(std::begin(data), std::end(data), A, B);

  const auto b = std::is_sorted(std::begin(data), std::end(data));
  if (!b)
    throw std::runtime_error("test_dist_count_sort");
}

int main()
{
  try {
    test_bubble_sort();
    test_comparison_counting_sort();
    test_tree_insertion_sort();
    std::cout << "test_inplace_comparison_counting_sort" << std::endl;
    test_inplace_comparison_counting_sort();
    std::cout << "Insertion sort." << std::endl;
    test_dist_count_sort();
    std::cout << "Insertion sort." << std::endl;
    test_straight_insertion();
    std::cout << "Straight selection." << std::endl;
    test_straight_selection();
    std::cout << "Merge sort." << std::endl;
    test_merge_sort();
    std::cout << "Test binary insertion sort." << std::endl;
    test_binary_insertion();
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}

