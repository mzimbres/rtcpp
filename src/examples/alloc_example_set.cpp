#include <array>

#include <rtcpp/utility/print.hpp>
#include <rtcpp/container/set.hpp>
#include <rtcpp/memory/node_allocator.hpp>

int main()
{
  using node_type = rt::set<int>::node_type;

  using alloc_type = rt::node_allocator<int, node_type>;

  const std::size_t offset = alloc_type::memory_use;
  std::array<char, offset + 100 * sizeof (node_type)> buffer = {{}};

  alloc_type alloc(buffer);
  //auto p = alloc.allocate(1);
  //auto p = alloc.allocate_node();

  using alloc_type2 = rt::node_allocator<node_type, node_type>;

  alloc_type2 aaa(alloc);
  auto p = aaa.allocate_node();
}

