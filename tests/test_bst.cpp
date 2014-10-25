#include <iostream>
#include <iterator>
#include <random>
#include <functional>
#include <algorithm>
#include <iterator>
#include <limits>
#include <array>
#include <vector>
#include <scoped_allocator>

#include <trees/node_stack.hpp>
#include <trees/bst.hpp>
#include <utility/make_rand_data.hpp>

int main()
{
  using namespace rtcpp;

  const int size = 400000;
  const int a = 1;
  const int b = std::numeric_limits<int>::max();
  // Use the limit below if you want the tree to be more unbalanced.
  // That is useful for benchmarks against balanced trees i.e. avl
  // or red-black for example.
  //const int b = size;

  // We have to know the node_type used by the tree in order to build an avail
  // stack.
  typedef bst<int>::node_type node_type;

  // The pool allocator type.
  typedef node_stack<node_type> allocator_type;

  // The container type.
  typedef bst<int, std::less<int>, allocator_type> set_type;

  // Random unique integers in the range [a,b].
  std::vector<int> tmp = make_rand_data(size, a, b);
  // The data size may be different from size as we remove repeated elements.
  std::cout << "Data size: " << tmp.size() << std::endl;

  // Node buffer
  std::vector<node_type> buffer(5 * size);

  // A pointer to the to of the node stack. All containers shall clain nodes
  // from this stack.
  node_type* const avail = link_stack(std::begin(buffer), std::end(buffer));

  node_stack<node_type> pool(avail);

  set_type t1(std::begin(tmp), std::end(tmp), pool);

  if (t1.size() != tmp.size())
    return 1;

  if (!std::is_sorted(std::begin(t1), std::end(t1)))
    return 1;

  set_type t2(pool);
  t1.copy(t2);

  set_type t3(t2);
  set_type t4 = t3;

  if (!std::equal(std::begin(t1), std::end(t1), std::begin(t2)))
    return 1;

  if (!std::equal(std::begin(t1), std::end(t1), std::begin(t3)))
    return 1;

  if (!std::equal(std::begin(t1), std::end(t1), std::begin(t4)))
    return 1;

  if (std::adjacent_find(std::begin(t1), std::end(t1)) != std::end(t1))
    return 1; // No duplicates allowed. (this must be improved)

  if (t2.size() != tmp.size())
    return 1;

  if (t3.size() != tmp.size())
    return 1;

  if (t4.size() != tmp.size())
    return 1;

  //std::copy(std::begin(t1), std::end(t1), std::ostream_iterator<int>(std::cout, " "));
  //std::cout << std::endl;
  //std::copy(t1.rbegin(), t1.rend(), std::ostream_iterator<int>(std::cout, " "));
  //std::cout << std::endl;

  t1.clear();
  if (!t1.empty())
    return 1;
    
  return 0;
}

