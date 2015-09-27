#include <set>
#include <array>
#include <list>
#include <forward_list>

#include <rtcpp/memory/node_allocator.hpp>
#include <rtcpp/utility/print.hpp>

  /*
  How to share the same buffer between containers that must be served with
  nodes of different sizes. I use two objects of each one of std::forward_list,
  std::list, std::set.
  */

int main()
{
  // The buffer that will be shared among all containers.
  const std::size_t s = 3000;
  std::array<char, 3 * s> buffer = {{}};

  // Allocators to serve different node types.
  // Alignment is handled automatically.
  rt::node_allocator<char> char_alloc(&buffer.front(), s);
  rt::node_allocator<int> int_alloc(&buffer.front() + s, s);
  rt::node_allocator<double> double_alloc(&buffer.front() + 2 * s, s);

  // For each type lets handle two containers.
  std::forward_list<char, rt::node_allocator<char>> c1(char_alloc);
  std::forward_list<char, rt::node_allocator<char>> c2(char_alloc);
  c1 = {'R', 'E', 'A', 'L', 'T', 'I', 'M', 'E'};
  c2 = {'A', 'L', 'L', 'O', 'C', 'A', 'T', 'O', 'R'};

  std::list<int, rt::node_allocator<int>> i1(int_alloc);
  std::list<int, rt::node_allocator<int>> i2(int_alloc);
  i1 = {1, 2, 3};
  i2 = {4, 5, 6};

  typedef std::set<double, std::less<double>, rt::node_allocator<double>> double_set_type;
  double_set_type d1(std::less<double>(), double_alloc);
  double_set_type d2(std::less<double>(), double_alloc);
  d1 = {3.4, 1.2, 5.6};
  d2 = {10.11, 7.8, 9.10};

  print(c1, "");
  print(c2, "");
  print(i1);
  print(i2);
  print(d1);
  print(d2);
}

