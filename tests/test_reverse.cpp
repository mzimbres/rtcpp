#include <array>
#include <iostream>
#include <iterator>
#include <algorithm>

#include <algorithm/reverse.hpp>

int main()
{
  using namespace rtcpp;

  std::array<int, 10> arr1 = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};
  std::array<int, 10> tmp1 = {{10, 9, 8, 7, 6, 5, 4, 3, 2, 1}};
  std::array<int, 11> arr2 = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}};
  std::array<int, 11> tmp2 = {{11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1}};

  reverse(std::begin(arr1), std::end(arr1));
  reverse(std::begin(arr2), std::end(arr2));

  if (!std::equal(std::begin(arr1), std::end(arr1), std::begin(tmp1)))
    return 1;

  if (!std::equal(std::begin(arr2), std::end(arr2), std::begin(tmp2)))
    return 1;

  return 0;
}

