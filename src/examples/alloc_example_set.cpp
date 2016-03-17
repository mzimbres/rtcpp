#include <iostream>
#include <functional>

#include <rtcpp/container/set.hpp>
#include <rtcpp/utility/print.hpp>
#include <rtcpp/memory/node_allocator.hpp>

int main()
{
  using node_type = typename rt::set<int>::node_type;
  using alloc_type = rt::node_allocator<int, node_type>;
  using set_type = rt::set<int, std::less<int>, alloc_type>;
  const auto r = alloc_type::reserved();

  std::array<char, r + 10 * sizeof (set_type::node_type)> buffer = {{}};
  alloc_type alloc(buffer);

  set_type t1(alloc);
  t1 = {5, 3, 7, 20, 1, 44, 10, 22, 8};
  print(t1);
  return 0;
}

