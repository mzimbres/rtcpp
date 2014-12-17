#include <iostream>
#include <iterator>
#include <random>
#include <functional>
#include <algorithm>
#include <iterator>
#include <limits>
#include <array>

#include <container/set.hpp>
#include <utility/make_rand_data.hpp>

int main()
{
  using namespace rt;

  std::array<int, 5> arr = {{5, 4, 3, 2, 1}};
  set<int> t1(std::begin(arr), std::end(arr));

  // Now the tree has three items 3, 2, 4. Lets test if the iterators can get
  // us to the right point.
  typedef std::reverse_iterator<set<int>::iterator> reverse_iter;
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

