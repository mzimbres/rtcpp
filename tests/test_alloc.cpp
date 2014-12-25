#include <iostream>
#include <iterator>
#include <functional>
#include <array>
#include <set>
#include <algorithm>

#include <memory/allocator.hpp>

int main()
{
  std::array<char, 8> buffer = {{}};
  rt::allocator<int> alloc(buffer);
  try {
    std::set<int, std::less<int>, rt::allocator<int>> t1(std::less<int>(), alloc);
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return 0;
  }
  return 1;

}

