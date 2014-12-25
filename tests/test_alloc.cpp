#include <iostream>
#include <iterator>
#include <functional>
#include <array>
#include <set>
#include <algorithm>

#include <memory/allocator.hpp>
#include <container/set.hpp>

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

int main()
{
  if (!test_buffer_size())
    return 1;

  if (!test_link_diff_node_size())
    return 1;

  return 0;
}

