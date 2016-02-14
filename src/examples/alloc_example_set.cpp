#include <array>

#include <rtcpp/utility/print.hpp>
#include <rtcpp/container/set.hpp>
#include <rtcpp/memory/node_allocator.hpp>

int main()
{
  // Relying on SCARY assignment of containers node type.
  using node_type = rt::set<int>::node_type;

  using alloc_type = rt::node_allocator<int, node_type>;
  const std::size_t offset = alloc_type::memory_use;

  using container_type = rt::set<int, std::less<int>, alloc_type>;

  std::array<char, offset + 100 * sizeof (node_type)> buffer = {{}};

  alloc_type alloc(buffer);

  container_type tmp(alloc);

  tmp = {5, 3, 0, 20, 99, 50, 7, 27, 1, 60};

  print(tmp);
}

