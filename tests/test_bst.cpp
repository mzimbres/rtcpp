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

#include <container/bst.hpp>

#include <utility/make_rand_data.hpp>

using namespace rt;

bool test_swap()
{
  bst<int> t1 = {2, 5, 9, 3, 0};
  bst<int> t1_copy = t1;
  bst<int> t2 = {1, 4, 8, 2, 7};
  bst<int> t2_copy = t2;
  //t1.swap(t2);
  std::swap(t1, t2);

  if (t1 != t2_copy)
    return false;

  if (t2 != t1_copy)
    return false;

  return true;
}

bool test_count()
{
  std::array<int, 5> arr = {{2, 5, 9, 3, 0}};
  bst<int> t1(std::begin(arr), std::end(arr));
  if (!std::all_of(std::begin(arr), std::end(arr), [&](int a){ return t1.count(a) == 1;}))
    return false;

  std::array<int, 5> tmp = {{1, 4, 8, 2, -1}};
  if (!std::any_of(std::begin(tmp), std::end(tmp), [&](int a){ return t1.count(a) == 0;}))
    return false;

  return true;
}

bool test_find()
{
  const std::array<int, 5> arr = {{2, 5, 9, 3, 0}};
  bst<int> t1(std::begin(arr), std::end(arr));
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
  const int size = 40000;
  const int a = 1;
  const int b = std::numeric_limits<int>::max();

  // Random unique integers in the range [a,b].
  std::vector<int> tmp = make_rand_data(size, a, b);

  std::vector<char> buffer(5 * (size + 2) * sizeof (bst<int>::node_type));

  bst<int> t1(std::begin(tmp), std::end(tmp), allocator<int>(&buffer[0], buffer.size()));

  if (t1.size() != tmp.size())
    return 1;

  if (!std::is_sorted(std::begin(t1), std::end(t1)))
    return 1;

  bst<int> t3(t1);
  bst<int> t4 = t3;

  if (t3 != t1)
    return 1;

  if (t4 != t1)
    return 1;

  if (std::adjacent_find(std::begin(t1), std::end(t1)) != std::end(t1))
    return 1;

  t1.clear();
  if (!t1.empty())
    return 1;

  if (!test_count())
    return 1;

  if (!test_find())
    return 1;

  if (!test_swap())
    return 1;
    
  //std::copy(std::begin(t1), std::end(t1), std::ostream_iterator<int>(std::cout, " "));
  //std::cout << std::endl;
  //std::copy(t1.rbegin(), t1.rend(), std::ostream_iterator<int>(std::cout, " "));
  //std::cout << std::endl;

  return 0;
}

