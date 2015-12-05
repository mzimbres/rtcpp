#pragma once

#include <iterator>
#include <functional>
#include <vector>

/*
   This is an implementation of the comparison counting sort.
   This is a O(n^2) algorithm.
*/

template <typename Iter>
void comp_counting_sort(Iter begin, Iter end)
{
  const auto n = std::distance(begin, end);
  std::vector<std::size_t> buffer(n, 0);

  for (auto i = n - 1; i > 0; --i)
    for (auto j = i - 1; j >= 0; --j)
      if (begin[i] < begin[j])
        ++buffer[j];
      else
        ++buffer[i];

  std::vector<std::size_t> buffer2(n, 0);
  for (auto i = 0; i < n; ++i)
    buffer2[buffer[i]] = begin[i];

  std::copy(buffer2.begin(), buffer2.end(), begin);
}

