#include <iostream>
#include <iterator>
#include <random>
#include <limits>
#include <functional>
#include <algorithm>

#include <trees/bst.hpp>

#include <utility/to_number.hpp>

int main()
{
  const int size = 40;
  const int a = 1;
  //const int b = std::numeric_limits<int>::max();
  const int b = size;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(a, b);

  bst<int> t1(size);

  int n = 0;
  while (n != size) {
    auto a = dis(gen);
    auto pair = t1.insert(a);
    if (pair.second)
      ++n;
  }

  if (std::distance(std::begin(t1), std::end(t1)) != size)
    return 1;

  if (!std::is_sorted(std::begin(t1), std::end(t1)))
    return 1;

  bst<int> t2(size);
  t1.copy(t2);

  if (!std::equal(std::begin(t1), std::end(t1), std::begin(t2)))
    return 1; // This is not enough to ensure equality between t1 and t2.

  if (std::adjacent_find(std::begin(t1), std::end(t1)) != std::end(t1))
    return 1; // No duplicates allowed.

  //std::copy(std::begin(t2), std::end(t2), std::ostream_iterator<int>(std::cout, " "));
  //std::cout << std::endl;
  //std::copy(std::begin(t1), std::end(t1), std::ostream_iterator<int>(std::cout, " "));
  //std::cout << std::endl;
  //std::copy(t1.rbegin(), t1.rend(), std::ostream_iterator<int>(std::cout, " "));
  //std::cout << std::endl;
    
  return 0;
}

