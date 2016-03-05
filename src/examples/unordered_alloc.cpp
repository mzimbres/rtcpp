#include <array>
#include <memory>

#include <rtcpp/memory/node_allocator.hpp>

template <typename T, typename Ptr>
struct node {
  using value_type = T;
  using pointer = typename std::pointer_traits<Ptr>::template rebind<node<T, Ptr>>;
  template<class U, class K>
  struct rebind { using other = node<U , K>; };
  char tmp[sizeof (char*)]; // just to make it big enough.
};

int main()
{
  // Simulates the container node type taken from an arbitrary
  // allocator. For example std::unordered_set<int>::node_type.
  // I am using an arbitrary pointer type as an example.
  using node_type1 = node<int, char*>;

  // User node allocator configured for node_type1.
  using alloc_type = rt::node_allocator<int, node_type1>;

  // Container specific node type. Known to the user only after
  // the container has been declared. For example
  // std::unordered_set<int, alloc_type>::node_type. It is equal
  // to node_type1 except (possibly) by the pointer type.
  using node_type2 = node<int, double*>;

  // Buffer for 100 elements
  std::array<char, 100 * sizeof (node_type2)> buffer = {{}};

  alloc_type alloc(buffer); // User allocator instance.

  // Not node_type2, array allocation is fine.
  auto a = alloc.allocate(10); // Ok

  // Error, node alocation not enabled for int.
  //auto b = alloc.allocate_node(); // Compile time error

  // The folowing lines ocurrs inside the unordered container.

  // Rebinds to get an allocator for node_type2.
  using node_alloc_type =
    typename alloc_type::template rebind<node_type2>::other;

  node_alloc_type node_alloc(alloc);

  // Error, array allocation not available for node_type.
  //auto c = node_alloc.allocate(10); // Compile time error

  // Ok, node allocation enabled.
  auto d = node_alloc.allocate_node(); // Ok

  // Rebinds to the unordered container internal array allocation
  // type. 
  using array_alloc_type =
    typename alloc_type::template rebind<float*>::other;

  array_alloc_type array_alloc(alloc);

  // Ok, Not the node type, array allocation enabled.
  auto e = alloc.allocate(10); // Compile time error

  // Error, node alocation is not enabled for float*.
  //auto f = alloc.allocate_node(); // Ok
  return 0;
}

