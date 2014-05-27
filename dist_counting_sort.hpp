#pragma once

#include <algorithm>
#include <iterator>
#include <functional>
#include <array>

#include "comp_counting_sort.hpp"

template <int A, int B, unsigned N, typename Iter>
bool dist_counting_sort(Iter begin)
{
  typedef typename std::iterator_traits<Iter>::difference_type diff_type;
  typedef typename std::iterator_traits<Iter>::value_type value_type;

  const unsigned count_size = B - A + 1;
  unsigned count[count_size] = {0};
  for (unsigned i = 0; i < N; ++i)
    count[begin[i] - A] += 1;

  std::partial_sum(&count[0], &count[0] + count_size, &count[0]);

  value_type out[N];
  for (int j = N - 1; j >= 0; --j) {
    const int idx = begin[j] - A;
    const int i = count[idx];
    out[i - 1] = begin[j];
    count[idx] = i - 1;
  }

  std::copy(&out[0], &out[0] + N, &begin[0]);
}

