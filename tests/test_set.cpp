#include <iostream>
#include <set>
#include <iterator>
#include <random>
#include <functional>
#include <algorithm>
#include <iterator>
#include <limits>
#include <array>
#include <vector>
#include <scoped_allocator>

#include <container/set.hpp>

#include <utility/make_rand_data.hpp>

template <typename C>
bool test_swap(const std::vector<int>& arr)
{
  C t1(std::begin(arr), std::end(arr));
  C t1_copy = t1;
  C t2 = {1, 4, 8, 2, 7};
  C t2_copy = t2;
  swap(t1, t2);
  //std::swap(t1, t2);

  if (t1 != t2_copy)
    return false;

  if (t2 != t1_copy)
    return false;

  return true;
}

template <typename C>
bool test_count(const std::vector<int>& arr)
{
  C t1(std::begin(arr), std::end(arr));
  if (!std::all_of(std::begin(arr), std::end(arr), [&](int a){ return t1.count(a) == 1;}))
    return false;

  return true;
}

template <typename C>
bool test_find(const std::vector<int>& arr)
{
  C t1(std::begin(arr), std::end(arr));
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

template <typename C>
bool test_basic(const std::vector<int>& tmp)
{
  C t1(std::begin(tmp), std::end(tmp));

  if (t1.size() != tmp.size())
    return false;

  if (!std::is_sorted(std::begin(t1), std::end(t1)))
    return false;

  C t3(t1);
  C t4 = t3;

  if (t3 != t1)
    return false;

  if (t4 != t1)
    return false;

  if (std::adjacent_find(std::begin(t1), std::end(t1)) != std::end(t1))
    return false;

  t1.clear();
  if (!t1.empty())
    return false;

  return true;
}

template <typename C>
bool run_tests(const std::vector<int>& tmp)
{
  if (!test_basic<C>(tmp))
    return false;

  if (!test_count<C>(tmp))
    return false;

  if (!test_find<C>(tmp))
    return false;

  if (!test_swap<C>(tmp))
    return false;

  return true;
}

int main()
{
  const int size = 40000;
  const int a = 1;
  const int b = std::numeric_limits<int>::max();

  // Random unique integers in the range [a,b].
  std::vector<int> tmp = rt::make_rand_data(size, a, b);

  if (!run_tests<rt::set<int>>(tmp))
    return 1;
    
  if (!run_tests<std::set<int>>(tmp))
    return 1;

  return 0;
}

