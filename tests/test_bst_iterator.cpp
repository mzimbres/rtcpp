#include <iostream>
#include <iterator>
#include <random>
#include <functional>
#include <algorithm>
#include <iterator>
#include <limits>
#include <array>

#include <trees/pool_allocator.hpp>
#include <trees/node_stack.hpp>
#include <trees/bst.hpp>
#include <utility/make_rand_data.hpp>

int main()
{
  using namespace rtcpp;

  typedef bst_node<int> node_type;
  typedef node_stack<node_type> pool_type;
  typedef pool_allocator<pool_type> allocator_type;
  typedef bst<int, std::less<int>, allocator_type> set_type;

  const int size = 5; // Space for three items.
  pool_type pool(size);
  allocator_type w(&pool);

  std::array<int, size> arr = {{5, 4, 3, 2, 1}};
  set_type t1(std::begin(arr), std::end(arr), w);

  // Now the tree has three items 3, 2, 4. Lets test if the iterators can get
  // us to the right point.

  typedef std::reverse_iterator<set_type::iterator> reverse_iter;
  reverse_iter iter(t1.end());

  // We should be now at the rbegin.
  if (*iter != 5)
    return 1;

  if (*++iter != 4)
    return 1;

  if (*++iter != 3)
    return 1;

  if (*++iter != 2)
    return 1;

  if (*++iter != 1)
    return 1;

  if (!std::equal(t1.rbegin(), t1.rend(), arr.begin()))
    return 1;

  if (!std::equal(t1.begin(), t1.end(), arr.rbegin()))
    return 1;

  return 0;
}

