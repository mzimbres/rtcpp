#include <array>
#include <set>
#include <list>
#include <forward_list>

#include <rtcpp/utility/print.hpp>
#include <rtcpp/container/set.hpp>
#include <rtcpp/memory/node_allocator.hpp>

/*
  In this example I use the rt::node_allocator to serve all standard node-based
  containers (and additionally my own implementation of std::set) from a single
  allocator instance. The allocator is optimal to serve std::set nodes, since
  it was the first to construct the rebound instance, but since the other
  containers rebind to smaller types, the allocators can also serve them,
  although not optimally, since it can only provide blocks with size equal to
  the type rebound by std::set.

  NOTE: I had to leave std::map and std::multimap for a moment since my compiler
  does not have support to the c++ interface of these containers.
*/

int main()
{
  std::array<char, 10000> buffer = {{}};

  rt::node_allocator<int> alloc(buffer);

  std::set<int, std::less<int>, rt::node_allocator<int>> t1(std::less<int>(), alloc);

  rt::set<int, std::less<int>, rt::node_allocator<int>> t2(alloc);

  std::multiset<int, std::less<int>, rt::node_allocator<int>> t3(std::less<int>(), alloc);

  std::list<int, rt::node_allocator<int>> t4(alloc);

  std::forward_list<int, rt::node_allocator<int>> t5(alloc);

  auto data = {5, 3, 5, 20, 3, 50, 7, 20, 1, 20};

  t1 = data; t2 = data; t3 = data; t4 = data; t5 = data;

  print(t1);
  print(t2);
  print(t3);
  print(t4);
  print(t5);
}

