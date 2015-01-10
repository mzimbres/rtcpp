#include <iostream>
#include <iterator>
#include <functional>
#include <array>
#include <set>
#include <algorithm>
#include <list>

#include <rtcpp/memory/allocator.hpp>
#include <rtcpp/container/set.hpp>

bool test_buffer_size()
{
  std::array<char, 1> buffer = {{}};
  rt::allocator<int> alloc(buffer);
  try {
    std::set<int, std::less<int>, rt::allocator<int>> t1(std::less<int>(), alloc);
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return true;
  }
  return false;
}

bool test_link_diff_node_size()
{
  std::array<char, 900> buffer = {{}};
  rt::allocator<int> alloc(buffer);
  try {
    std::set<int, std::less<int>, rt::allocator<int>> t1(std::less<int>(), alloc);
    rt::set<int, std::less<int>, rt::allocator<int>> t2(alloc);
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return true;
  }
  return false;
}

bool test_list()
{
  std::array<int, 6> data = {{10, 3, 2, 8, 19, 33}};

  std::array<char, 5000> buffer = {{}};

  rt::allocator<int> alloc(buffer);

  std::list<int, rt::allocator<int>> t1(std::begin(data), std::end(data), alloc);
  std::list<int, rt::allocator<int>> t2(std::begin(data), std::end(data), alloc);
  std::list<int, rt::allocator<int>> t3(std::begin(data), std::end(data), alloc);

  if (!std::equal(std::begin(data), std::end(data), std::begin(t1)))
    return false;

  if (!std::equal(std::begin(data), std::end(data), std::begin(t2)))
    return false;

  if (!std::equal(std::begin(data), std::end(data), std::begin(t3)))
    return false;

  return true;
}

bool test_deallocate()
{
  const std::size_t ptr_size = sizeof (char*);
  const std::size_t data_size = sizeof (std::size_t);
  std::array<char, 3 * ptr_size + 2 * data_size> buffer = {{}};

  rt::allocator<int> alloc1(buffer);
  rt::allocator<std::size_t> alloc2(alloc1); // links the allocator.

  std::size_t* p1 = alloc2.allocate(1);
  *p1 = 10; // To avoid a warning.
  std::size_t* p2 = alloc2.allocate(1);
  try {
    std::size_t* p3 = alloc2.allocate(1); // Should throw.
    *p3 = 100; // To avoid a warning.
    return false;
  } catch (...) {
    alloc2.deallocate(p2, 1);
  }
  std::size_t* p = alloc2.allocate(1);
  *p = 2;
  if (*p != 2)
    return false;
  return true;
}

int main()
{
  try {
    if (!test_buffer_size())
      return 1;

    if (!test_link_diff_node_size())
      return 1;

    if (!test_list())
      return 1;

    if (!test_deallocate())
      return 1;
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return 1;
  }
  return 0;
}

