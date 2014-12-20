#include <iostream>
#include <iterator>
#include <functional>
#include <array>
#include <set>
#include <algorithm>

#include <memory/allocator.hpp>

int main()
{
  std::array<char, 10000> buffer = {{}};
  rt::allocator<int> alloc(buffer);
  std::set<int, std::less<int>, rt::allocator<int>> t1(std::less<int>(), alloc);

  std::array<int, 12> data = {{4, 2, 9, 10, 7, 88, 44, 31, 22, 35, 96, 100}};

  t1.insert(std::begin(data), std::end(data));;

  std::sort(std::begin(data), std::end(data));
  if (!std::equal(std::begin(t1), std::end(t1), std::begin(data)))
    return 1;

  std::copy(std::begin(t1), std::end(t1), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
}

