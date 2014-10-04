#include <iostream>
#include <iterator>
#include <random>
#include <functional>
#include <algorithm>
#include <iterator>
#include <limits>

#include <trees/bst.hpp>
#include <utility/make_rand_data.hpp>

  //std::copy(std::begin(tmp), std::end(tmp), std::ostream_iterator<int>(std::cout, " "));
  //std::cout << std::endl;

int main()
{
  using namespace rtcpp;

  const int size = 40;
  const int a = 1;
  //const int b = std::numeric_limits<int>::max();
  const int b = size;

  std::vector<int> tmp = make_rand_data(size, a, b);
  node_pool<int> pool(4 * size);
  bst<int> t1(std::begin(tmp), std::end(tmp), pool);

  if (std::distance(std::begin(t1), std::end(t1)) != std::distance(std::begin(tmp), std::end(tmp)))
    return 1;

  if (!std::is_sorted(std::begin(t1), std::end(t1)))
    return 1;

  bst<int> t2(pool);
  t1.copy(t2);

  rtcpp::bst<int> t3(t2);
  rtcpp::bst<int> t4 = t3;

  if (!std::equal(std::begin(t1), std::end(t1), std::begin(t2)))
    return 1;

  if (!std::equal(std::begin(t1), std::end(t1), std::begin(t3)))
    return 1;

  if (!std::equal(std::begin(t1), std::end(t1), std::begin(t4)))
    return 1;

  if (std::adjacent_find(std::begin(t1), std::end(t1)) != std::end(t1))
    return 1; // No duplicates allowed. (this must be improved)

  std::sort(std::begin(tmp), std::end(tmp));

  if (!std::equal(t1.rbegin(), t1.rend(), tmp.rbegin()))
    return 1;

  std::copy(std::begin(t1), std::end(t1), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
  std::copy(t1.rbegin(), t1.rend(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
    
  return 0;
}

