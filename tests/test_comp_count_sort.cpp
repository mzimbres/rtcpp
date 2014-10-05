#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <functional>

#include <utility/make_rand_data.hpp>
#include <sort/comp_counting_sort.hpp>

int main()
{
  using namespace rtcpp;
  const int size = 50;
  const int a = 1;
  const int b = 20;
  std::vector<int> vec = make_rand_data(size, a, b, 1);
  //std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
  //std::cout << std::endl;
  comp_counting_sort(std::begin(vec), std::end(vec));
  if (!std::is_sorted(vec.begin(), vec.end()))
    return 1;

  return 0;
}

