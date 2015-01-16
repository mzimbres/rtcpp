#include <array>
#include <list>

#include <rtcpp/container/set.hpp>
#include <rtcpp/memory/allocator.hpp>
#include <rtcpp/utility/print.hpp>

int main()
{
  std::array<char, 2000> buffer = {{}};
  rt::allocator<int> alloc(buffer);

  std::list<int, rt::allocator<int>> t1(alloc);
  t1 = {5, 3, 7, 20, 1, 44, 22, 8};

  print(t1);
}

