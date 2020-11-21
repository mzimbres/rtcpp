#include <array>
#include <vector>
#include <limits>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <forward_list>

#include "rtcpp.hpp"
#include "test.hpp"

RT_TEST(test_find_intrusive1)
{
  std::vector<int> data {2, 4, 6, 9, 3, 12};
  rt::print(data);

  for (unsigned i = 0; i < data.size() - 1; ++i) {
    data.back() = data[i];
    auto iter = rt::find_with_sentinel(std::begin(data), data[i]);
    std::cout << "K = " << data[i] << "\n";
    if (iter == std::end(data))
      throw std::runtime_error(f);
  }
}

RT_TEST(test_find_intrusive2)
{
  std::vector<int> data {2, 4, 6, 9, 3, 12};
  rt::print(data);

  // Searches for a number that is not in the array.
  data.back() = 13;
  auto iter = rt::find_with_sentinel(std::begin(data), 13);
  RT_CHECK(*iter == data.back())
}

RT_TEST(test_reverse)
{
  using namespace rt;

  std::array<int, 10> arr1 = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};
  std::array<int, 10> tmp1 = {{10, 9, 8, 7, 6, 5, 4, 3, 2, 1}};
  std::array<int, 11> arr2 = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}};
  std::array<int, 11> tmp2 = {{11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1}};

  rt::reverse(std::begin(arr1), std::end(arr1));
  rt::reverse(std::begin(arr2), std::end(arr2));

  RT_CHECK(std::equal(std::begin(arr1), std::end(arr1), std::begin(tmp1)))
  RT_CHECK(std::equal(std::begin(arr2), std::end(arr2), std::begin(tmp2)))
}

RT_TEST(test_binary_search)
{
  std::array<int, 10> data = {1, 20, 32, 44, 51, 69, 70, 87, 91, 101};

  for (unsigned i = 0; i < data.size(); ++i)
    RT_CHECK(rt::binary_search(std::begin(data), std::end(data), data[i]))

  RT_CHECK(!rt::binary_search(std::begin(data), std::end(data), 10))
}

RT_TEST(test_binary_search2)
{
  std::forward_list<int> data {1, 20, 32, 44, 51, 69, 70, 87, 91, 101};

  for (auto o : data)
    RT_CHECK(rt::binary_search2(std::begin(data), std::end(data), o))

  RT_CHECK(!rt::binary_search2(std::begin(data), std::end(data), 10))
}

RT_TEST(test_binary_search_rec)
{
  std::vector<int> data {1, 20, 32, 44, 51, 69, 70, 87, 91, 101};

  for (std::size_t i = 0; i < data.size(); ++i)
    RT_CHECK(rt::binary_search_recursive( std::begin(data)
                                        , std::end(data), data[i]))

  RT_CHECK(!rt::binary_search_recursive( std::begin(data)
                                       , std::end(data), 10))
}

RT_TEST(test_binary_search_rotated)
{
  (void)f;
  std::array<int, 10> data = {5, 7, 9, 13, 15, 19, 1, 2, 3, 4};

  for (int i = 0; i < 25; ++i) {
    auto b = rt::binary_search_rotated( std::begin(data)
                                      , std::end(data), i);
    std::cout << i << ": " << b << std::endl;
  }
}

RT_TEST(test_find)
{
  std::array<int, 10> data = {5, 7, 9, 13, 15, 19, 1, 2, 3, 4};

  for (unsigned i = 0; i < data.size(); ++i) {
    auto match = rt::find(std::begin(data), std::end(data), data[i]);
    if (match == std::end(data))
      throw std::runtime_error(f);
  }

  auto match = rt::find(std::begin(data), std::end(data), 222);
  if (match != std::end(data))
    throw std::runtime_error(f);

  // Empty range
  std::vector<int> tmp;
  auto match2 = rt::find(std::begin(tmp), std::end(tmp), 222);
  if (match2 != std::end(tmp))
    throw std::runtime_error(f);

  std::cout << "test_find ok" << std::endl;
}

RT_TEST(test_max_element)
{
  std::array<int, 10> data = {5, 7, 9, 13, 15, 19, 1, 2, 3, 4};

  auto max = rt::max_element(std::begin(data), std::end(data));

  if (max == std::end(data))
    throw std::runtime_error(f);

  if (*max != 19)
    throw std::runtime_error(f);

  std::cout << "test_max_element ok" << std::endl;
}

RT_TEST(test_min_element)
{
  std::array<int, 10> data = {5, 7, 9, 13, 15, 19, 1, 2, 3, 4};

  auto min = rt::min_element(std::begin(data), std::end(data));

  RT_CHECK(min != std::end(data))
  RT_CHECK(*min == 1)

  std::cout << "test_min_element ok" << std::endl;
}

int main()
{
  try {
    test_binary_search();
    test_binary_search2();
    test_binary_search_rec();
    test_binary_search_rotated();
    test_reverse();
    test_find_intrusive1();
    test_find_intrusive2();
    test_find();
    test_max_element();
    test_min_element();
  } catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  } catch (...) {
    std::cerr << "Error" << std::endl;
    return 1;
  }

  return 0;
}

