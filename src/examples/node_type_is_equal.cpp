#include <iostream>
#include <functional>

#include <rtcpp/container/set.hpp>
#include <rtcpp/memory/node_allocator_lazy.hpp>

int main()
{
  using T = int;
  using C1 = std::less<T>;
  using C2 = std::greater<T>;
  using A1 = std::allocator<T>;
  using A2 = rt::node_allocator_lazy<T>;

  using set_type1 = rt::set<T, C1, A1>;
  using set_type2 = rt::set<T, C2, A2>;

  using pointer = double*; // Arbitray pointer type.
  using node_type1 = typename set_type1::node_type::template rebind<T, pointer>;
  using node_type2 = typename set_type2::node_type::template rebind<T, pointer>;
  static_assert(std::is_same<node_type1, node_type2>::value, "Not same");

  std::cout << std::is_same<node_type1, node_type2>::value << std::endl;
  return 0;
}

