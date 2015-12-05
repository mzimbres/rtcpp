#pragma once

#include <algorithm>
#include <iterator>
#include <functional>
#include <vector>

#include "comp_counting_sort.hpp"

template <typename Iter>
void dist_counting_sort( Iter begin
                       , std::size_t N
                       , std::size_t A
                       , std::size_t B)
{
  const std::size_t count_size = B - A + 1;
  std::vector<std::size_t> count(count_size, 0);
  for (std::size_t i = 0; i < N; ++i)
    count[begin[i] - A] += 1;

  std::partial_sum( std::begin(count)
                  , std::end(count), std::begin(count));

  typedef typename std::iterator_traits<Iter>::value_type value_type;
  std::vector<value_type> out(N, 0);
  for (int j = N - 1; j >= 0; --j) {
    const int idx = begin[j] - A;
    const int i = count[idx];
    out[i - 1] = begin[j];
    count[idx] = i - 1;
  }

  std::copy(std::begin(out), std::end(out), begin);
}

