#include <array>

#include <rtcpp/utility/print.hpp>
#include <rtcpp/container/set.hpp>
#include <rtcpp/memory/node_allocator_lazy.hpp>

int main()
{
  using alloc_type = rt::node_allocator_lazy<int>;
  const std::size_t alloc_memory_use = alloc_type::memory_use;

  using container_type = rt::set<int, std::less<int>, alloc_type>;
  using node_type = typename container_type::node_type;
  const std::size_t node_size = sizeof (node_type);

  std::array<char, alloc_memory_use + 11 * node_size> buffer = {{}};

  alloc_type alloc(buffer);

  container_type tmp(alloc);

  tmp = {5, 3, 0, 20, 99, 50, 7, 27, 1, 60};

  print(tmp);
}

