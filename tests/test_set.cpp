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

#include <container/set.hpp>
#include <memory/allocator.hpp>

#include <utility/make_rand_data.hpp>

bool test_swap(const std::vector<int>& arr)
{
  rt::set<int> t1(std::begin(arr), std::end(arr));
  rt::set<int> t1_copy = t1;
  rt::set<int> t2 = {1, 4, 8, 2, 7};
  rt::set<int> t2_copy = t2;
  std::swap(t1, t2);

  if (t1 != t2_copy)
    return false;

  if (t2 != t1_copy)
    return false;

  return true;
}

template <typename C>
bool test_count(C& t1, const std::vector<int>& arr)
{
  t1.clear();
  t1.insert(std::begin(arr), std::end(arr));
  if (!std::all_of(std::begin(arr), std::end(arr), [&](int a){ return t1.count(a) == 1;}))
    return false;

  return true;
}

template <typename C>
bool test_find(C& t1,const std::vector<int>& arr)
{
  t1.clear();
  t1.insert(std::begin(arr), std::end(arr));
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
bool test_basic(C& t1, const std::vector<int>& tmp)
{
  t1.clear();
  t1.insert(std::begin(tmp), std::end(tmp));

  if (t1.size() != tmp.size())
    return false;

  if (!std::is_sorted(std::begin(t1), std::end(t1)))
    return false;

  C t3(t1);
  if (t3 != t1)
    return false;

  C t4 = t3;
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
bool run_tests(C& t1, const std::vector<int>& tmp)
{
  if (!test_basic(t1, tmp))
    return false;

  if (!test_count(t1, tmp))
    return false;

  if (!test_find(t1, tmp))
    return false;

  if (!test_swap(tmp))
    return false;

  return true;
}

int main()
{
  const int size = 500;
  const int a = 1;
  const int b = std::numeric_limits<int>::max();

  // Random unique integers in the range [a,b].
  std::vector<int> tmp = rt::make_rand_data(size, a, b);

  const int bsize = 3 * (tmp.size() + 2) * 25; // Should be enough for rt::set.

  // Tow buffers for two allocators.
  std::vector<char> buffer1(2 * bsize);
  std::vector<char> buffer2(4 * bsize);

  rt::allocator<int> alloc1(buffer1);
  rt::allocator<int> alloc2(buffer2);

  rt::set<int> t1;
  rt::set<int, std::less<int>, rt::allocator<int>> t2(std::less<int>(), alloc1);
  rt::set<int, std::less<int>, rt::allocator<int>> t3(std::less<int>(), alloc1);
  std::set<int> t4;
  std::set<int, std::less<int>, rt::allocator<int>> t5(std::less<int>(), alloc2);
  std::set<int, std::less<int>, rt::allocator<int>> t6(std::less<int>(), alloc2);

  if (!run_tests(t1, tmp))
    return 1;
    
  if (!run_tests(t2, tmp))
    return 1;

  if (!run_tests(t3, tmp))
    return 1;

  if (!run_tests(t4, tmp))
    return 1;

  if (!run_tests(t5, tmp))
    return 1;

  if (!run_tests(t6, tmp))
    return 1;

  return 0;
}

