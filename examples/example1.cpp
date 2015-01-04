#include <iostream>
#include <iomanip>
#include <array>
#include <set>

#include <rtcpp/container/set.hpp>
#include <rtcpp/memory/allocator.hpp>

int main()
{
  typedef std::set<int, std::less<int>, rt::allocator<int>> rt_set_type;

  std::array<char, 2000> buffer = {{}};
  rt::allocator<int> alloc(buffer);

  rt_set_type t1(std::less<int>(), alloc);
  t1 = {5, 3, 7, 20, 1, 44, 22, 8};

  std::copy( std::begin(t1)
           , std::end(t1)
           , std::ostream_iterator<int>(std::cout, " "));

  std::cout << std::endl;
}

