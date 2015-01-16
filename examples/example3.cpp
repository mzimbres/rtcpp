#include <array>
#include <set>

#include <rtcpp/container/set.hpp>
#include <rtcpp/memory/allocator.hpp>
#include <rtcpp/utility/print.hpp>

  /*
  How to share the same buffer between containers that must be served with
  nodes of different sizes.
  */

int main()
{
  // Sets that must be served with nodes of different sizes.
  typedef std::set<char, std::less<char>, rt::allocator<char>> char_set_type;
  typedef std::set<int, std::less<int>, rt::allocator<int>> int_set_type;
  typedef std::set<double, std::less<double>, rt::allocator<double>> double_set_type;

  // The buffer that will be shared among all sets.
  const std::size_t s = 3000;
  std::array<char, 3 * s> buffer = {{}};

  // Allocators to serve different node types.
  // Alignment s handled automatically.
  rt::allocator<char> char_alloc(&buffer.front(), s);
  rt::allocator<int> int_alloc(&buffer.front() + s, s);
  rt::allocator<double> double_alloc(&buffer.front() + 2 * s, s);

  // For each type lets handle two containers.
  char_set_type c1(char_alloc);
  char_set_type c2(char_alloc);
  c1 = {'F', 'O', 'O'};
  c2 = {'B', 'A', 'R'};

  int_set_type i1(int_alloc);
  int_set_type i2(int_alloc);
  i1 = {1, 2, 3};
  i2 = {4, 5, 6};

  double_set_type d1(double_alloc);
  double_set_type d2(double_alloc);
  d1 = {1.2, 3.4, 5.6};
  d2 = {7.8, 9.10, 10.11};

  print(c1);
  print(c2);
  print(i1);
  print(i2);
  print(d1);
  print(d2);
}

