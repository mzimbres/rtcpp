#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iterator>
#include <functional>
#include <array>

#include <sort/comp_counting_sort.hpp>
#include <sort/dist_counting_sort.hpp>

int main()
{
  const int size = 50;
  const int a = 1;
  const int b = 20;
  std::vector<int> vec(size);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::generate(vec.begin(), vec.end(), std::bind(std::uniform_int_distribution<>(a, b), gen));
  std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
  comp_counting_sort(std::begin(vec), std::end(vec));
  std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
  if (!std::is_sorted(vec.begin(), vec.end()))
    return 1;

  //for (int i = 0; i < loop_size; ++i) {
  //  std::generate(vec.begin(), vec.end(), std::bind(std::uniform_int_distribution<>(a, b), gen));
  //  dist_counting_sort<a, b, size>(vec.begin());
  //}

  //for (int i = 0; i < loop_size; ++i) {
  //  std::generate(vec.begin(), vec.end(), std::bind(std::uniform_int_distribution<>(a, b), gen));
  //  std::sort(vec.begin(), vec.end());
  //}

  return 0;
}

