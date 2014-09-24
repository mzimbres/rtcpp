#pragma once

#include <iterator>
#include <functional>
#include <algorithm>
#include <array>
#include <vector>

  // Comparison counting sort. The most straightforward sorting algorithm.
  // I am implementing this just to as warm up for other algorithms.
  // This implementation is certainly not realtime since to buffers are
  // dynamically allocated throw std::vector (using default allocator).

template <typename Iter>
void comp_counting_sort(Iter begin, Iter end)
{
  typedef typename std::iterator_traits<Iter>::value_type value_type;
  typedef typename std::iterator_traits<Iter>::difference_type diff_type;
  const auto n = std::distance(begin, end);
  std::vector<std::size_t> buffer(n, 0);

  for (diff_type i = n - 1; i > 0; --i)
    for (diff_type j = i - 1; j >= 0; --j)
      if (begin[i] < begin[j])
        ++buffer[j];
      else
        ++buffer[i];

  std::vector<std::size_t> buffer2(n, 0);
  for (diff_type i = 0; i < n; ++i)
    buffer2[buffer[i]] = begin[i];

  std::copy(buffer2.begin(), buffer2.end(), begin);
}

