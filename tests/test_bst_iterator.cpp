#include <iostream>
#include <iterator>
#include <random>
#include <functional>
#include <algorithm>
#include <iterator>
#include <limits>
#include <array>

#include <container/bst.hpp>
#include <utility/make_rand_data.hpp>

int main()
{
  using namespace rtcpp;


  const int size = 5; // Space for three items.

  typedef bst_node<int> node_type;
  std::vector<char> buffer(size * sizeof (node_type));

  allocator<int> alloc(&buffer[0], buffer.size());

  std::array<int, size> arr = {{5, 4, 3, 2, 1}};
  bst<int> t1(std::begin(arr), std::end(arr), alloc);

  // Now the tree has three items 3, 2, 4. Lets test if the iterators can get
  // us to the right point.
  typedef std::reverse_iterator<bst<int>::iterator> reverse_iter;
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

