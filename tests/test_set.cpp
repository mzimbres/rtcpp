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
#include <ext/pool_allocator.h>
#include <ext/bitmap_allocator.h>
#include <ext/mt_allocator.h>

#include <rtcpp/container/set.hpp>
#include <rtcpp/memory/allocator.hpp>
#include <rtcpp/utility/make_rand_data.hpp>

template <typename T>
bool test_iterator()
{
  std::array<T, 5> arr = {{5, 10, -3, 32, 41}};
  rt::set<T> t1(std::begin(arr), std::end(arr));

  // Now the tree has three items 3, 2, 4. Lets test if the iterators can get
  // us to the right point.
  auto iter = t1.end();

  // We should be now at the rbegin.
  if (*--iter != 41)
    return false;

  if (*--iter != 32)
    return false;

  if (*--iter != 10)
    return false;

  if (*--iter != 5)
    return false;

  if (*--iter != -3)
    return false;

  return true;
}

template <typename T>
bool test_swap(const std::vector<T>& arr)
{
  rt::set<T> t1(std::begin(arr), std::end(arr));
  rt::set<T> t1_copy = t1;
  rt::set<T> t2 = {1, 4, 8, 2, 7};
  rt::set<T> t2_copy = t2;
  std::swap(t1, t2);

  if (t1 != t2_copy)
    return false;

  if (t2 != t1_copy)
    return false;

  return true;
}

template <typename C>
bool test_count(C& t1, const std::vector<typename C::value_type>& arr)
{
  typedef typename C::value_type value_type;
  t1.clear();
  t1.insert(std::begin(arr), std::end(arr));
  if (!std::all_of(std::begin(arr), std::end(arr), [&](value_type a){ return t1.count(a) == 1;}))
    return false;

  return true;
}

template <typename C>
bool test_find(C& t1,const std::vector<typename C::value_type>& arr)
{
  typedef typename C::value_type value_type;
  t1.clear();
  t1.insert(std::begin(arr), std::end(arr));
  auto func = [&](value_type a) -> bool
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
bool test_basic(C& t1, const std::vector<typename C::value_type>& tmp)
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
bool run_tests(C& t1, const std::vector<typename C::value_type>& tmp)
{
  typedef typename C::value_type value_type;
  if (!test_basic(t1, tmp))
    return false;

  if (!test_count(t1, tmp))
    return false;

  if (!test_find(t1, tmp))
    return false;

  if (!test_swap<value_type>(tmp))
    return false;

  if (!test_iterator<value_type>())
    return false;

  return true;
}

template <typename T>
bool run_tests_all()
{
  const T size = 500;
  const int a = 1;
  const int b = std::numeric_limits<int>::max();

  // Random unique integers in the range [a,b].
  std::vector<T> tmp = rt::make_rand_data<T>(size, a, b);

  const T bsize = 3 * (tmp.size() + 2) * 25; // Should be enough for rt::set.

  // Tow buffers for two allocators.
  std::vector<char> buffer1(2 * bsize);
  std::vector<char> buffer2(4 * bsize);

  rt::allocator<T> alloc1(buffer1);
  rt::allocator<T> alloc2(buffer2);

  rt::set<T> t1;
  rt::set<T, std::less<T>, rt::allocator<T>> t2(std::less<T>(), alloc1);
  rt::set<T, std::less<T>, rt::allocator<T>> t3(std::less<T>(), alloc1);
  rt::set<T, std::less<T>, __gnu_cxx::__pool_alloc<T>> t4;
  rt::set<T, std::less<T>, __gnu_cxx::bitmap_allocator<T>> t5;
  rt::set<T, std::less<T>, __gnu_cxx::__mt_alloc<T>> t6;

  std::set<T> t7;
  std::set<T, std::less<T>, rt::allocator<T>> t8(std::less<T>(), alloc2);
  std::set<T, std::less<T>, rt::allocator<T>> t9(std::less<T>(), alloc2);
  std::set<T, std::less<T>, __gnu_cxx::__pool_alloc<T>> t10;
  std::set<T, std::less<T>, __gnu_cxx::bitmap_allocator<T>> t11;
  std::set<T, std::less<T>, __gnu_cxx::__mt_alloc<T>> t12;

  if (!run_tests(t1, tmp))
    return false;
    
  if (!run_tests(t2, tmp))
    return false;

  if (!run_tests(t3, tmp))
    return false;

  if (!run_tests(t4, tmp))
    return false;

  if (!run_tests(t5, tmp))
    return false;

  if (!run_tests(t6, tmp))
    return false;

  if (!run_tests(t7, tmp))
    return false;

  if (!run_tests(t8, tmp))
    return false;

  if (!run_tests(t9, tmp))
    return false;

  if (!run_tests(t10, tmp))
    return false;

  if (!run_tests(t11, tmp))
    return false;

  if (!run_tests(t12, tmp))
    return false;

  return true;
}

int main()
{
  const bool b1 = run_tests_all<int>();
  const bool b2 = run_tests_all<long long int>();
  return (b1 && b2) ? 0 : 1;
}

