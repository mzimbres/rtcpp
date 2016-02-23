#include <array>

#include <rtcpp/utility/print.hpp>
#include <rtcpp/container/set.hpp>
#include <rtcpp/memory/node_allocator.hpp>

int main()
{
  // Buffer where nodes are allocated.
  std::array<char, 1000> buffer = {{}};

  // SCARY aware node type. It does not matter from which allcator
  // it was taken since we can rebind it.
  using node_type = rt::set<int>::node_type;

  // The user node allocator type and its instance.
  using alloc_type = rt::node_allocator<int, node_type>;
  alloc_type alloc(buffer);

  // Ok, This is not the node_type, we can perform array allcation.
  auto a = alloc.allocate(10);

  // Error, node alocation is not enabled for int.
  //auto a = alloc.allocate_node(); // Compile time error

  // The folowing lines ocurrs inside the unordered container.

  // Rebinds to the unordered container internal node_type. 
  using node_alloc_type =
    typename alloc_type::template rebind<node_type>::other;

  node_alloc_type node_alloc(alloc);

  // Error, this allocator cannot perform array allocation for
  // node_type.
  //auto b = node_alloc.allocate(10); // Error

  // Ok, The allocator was configured to perform node allocation
  // on this type.
  auto b = node_alloc.allocate_node(); // Ok

  // Rebinds to the unordered container internal array allocation
  // type, let us say it is double* (just an example). 
  using array_alloc_type =
    typename alloc_type::template rebind<double*>::other;

  array_alloc_type array_alloc(alloc);

  // Ok, This is not the node_type, we can perform array allcation.
  auto c = alloc.allocate(10); // Error

  // Error, node alocation is not enabled for double*.
  //auto c = alloc.allocate_node(); // Ok
}

