#include <iostream>
#include <iterator>
#include <random>
#include <functional>
#include <algorithm>
#include <iterator>
#include <limits>
#include <array>
#include <vector>
#include <scoped_allocator>

#include <trees/bst.hpp>

#include <utility/make_rand_data.hpp>

using namespace rtcpp;

bool test_count()
{
  const std::size_t size = 5;
  const std::size_t node_size = sizeof (bst<int>::node_type);

  std::array<char, size * node_size> buffer; // Buffer.

  std::array<int, 5> arr = {{2, 5, 9, 3, 0}};
  bst<int> t1(std::begin(arr), std::end(arr), allocator<int>(&buffer[0], buffer.size()));
  if (!std::all_of(std::begin(arr), std::end(arr), [&](int a){ return t1.count(a) == 1;}))
    return false;

  std::array<int, 5> tmp = {{1, 4, 8, 2, -1}};
  if (!std::any_of(std::begin(tmp), std::end(tmp), [&](int a){ return t1.count(a) == 0;}))
    return false;

  return true;
}

bool test_find()
{
  const std::size_t size = 5;
  const std::size_t node_size = sizeof (bst<int>::node_type);

  std::array<char, size * node_size> buffer; // Buffer.

  const std::array<int, 5> arr = {{2, 5, 9, 3, 0}};
  bst<int> t1(std::begin(arr), std::end(arr), allocator<int>(&buffer[0], buffer.size()));
  auto func = [&](int a) -> bool
  {
    auto iter = t1.find(a);
    if (iter == t1.end())
      return false;
    if (*iter != a)
      return false;
    return true;
  };
  const bool b1 = std::all_of(std::begin(arr), std::end(arr), func);
  if (!b1)
    return false;

  return true;
}


int main()
{
  const int size = 400000;
  const int a = 1;
  const int b = std::numeric_limits<int>::max();

  // Random unique integers in the range [a,b].
  std::vector<int> tmp = make_rand_data(size, a, b);

  std::vector<char> buffer(5 * size * sizeof (bst<int>::node_type));

  allocator<int> alloc(&buffer[0], buffer.size());

  bst<int> t1(std::begin(tmp), std::end(tmp), std::ref(alloc));

  if (t1.size() != tmp.size())
    return 1;

  if (!std::is_sorted(std::begin(t1), std::end(t1)))
    return 1;

  bst<int> t3(t1);
  bst<int> t4 = t3;

  if (!std::equal(std::begin(t1), std::end(t1), std::begin(t3)))
    return 1;

  if (!std::equal(std::begin(t1), std::end(t1), std::begin(t4)))
    return 1;

  if (std::adjacent_find(std::begin(t1), std::end(t1)) != std::end(t1))
    return 1; // No duplicates allowed. (this must be improved)

  if (t3.size() != tmp.size())
    return 1;

  if (t4.size() != tmp.size())
    return 1;

  //std::copy(std::begin(t1), std::end(t1), std::ostream_iterator<int>(std::cout, " "));
  //std::cout << std::endl;
  //std::copy(t1.rbegin(), t1.rend(), std::ostream_iterator<int>(std::cout, " "));
  //std::cout << std::endl;

  t1.clear();
  if (!t1.empty())
    return 1;

  if (!test_count())
    return 1;

  if (!test_find())
    return 1;
    
  return 0;
}

