#include <array>
#include <list>

#include <rtcpp/memory/node_allocator.hpp>
#include <rtcpp/utility/print.hpp>

int main()
{
  std::array<char, 2000> buffer = {{}};
  rt::node_allocator<int> alloc(buffer);

  std::list<int, rt::node_allocator<int>> t1(alloc);
  t1 = {5, 3, 7, 20, 1, 44, 22, 8};

  print(t1);
}

