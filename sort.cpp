#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iterator>
#include <functional>
#include <array>

#include "comp_counting_sort.hpp"
#include "dist_counting_sort.hpp"

int main()
{
  const int size = 50000;
  const int a = 1;
  const int b = 20;
  std::array<int, size> vec;
  std::random_device rd;
  std::mt19937 gen(rd());
  //std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
  //std::cout << std::endl;

  const int loop_size = 3000;
  //for (int i = 0; i < loop_size; ++i) {
  //  std::generate(vec.begin(), vec.end(), std::bind(std::uniform_int_distribution<>(a, b), gen));
  //  comp_counting_sort<size>(vec.begin());
  //}

  for (int i = 0; i < loop_size; ++i) {
    std::generate(vec.begin(), vec.end(), std::bind(std::uniform_int_distribution<>(a, b), gen));
    dist_counting_sort<a, b, size>(vec.begin());
  }

  //for (int i = 0; i < loop_size; ++i) {
  //  std::generate(vec.begin(), vec.end(), std::bind(std::uniform_int_distribution<>(a, b), gen));
  //  std::sort(vec.begin(), vec.end());
  //}

  //std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
  //std::cout << std::endl;
  return 0;
}

