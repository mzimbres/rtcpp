#pragma once

#include <algorithm>
#include <iterator>
#include <functional>
#include <vector>

#include "comp_counting_sort.hpp"

template <typename Iter>
void dist_counting_sort( Iter begin
                       , std::size_t N
                       , typename std::iterator_traits<Iter>::value_type A
                       , typename std::iterator_traits<Iter>::value_type B)
{
  const std::size_t count_size = B - A + 1;
  std::vector<std::size_t> count(count_size, 0);
  // Counts the number of elements of each value.
  for (std::size_t i = 0; i < N; ++i)
    ++count[begin[i] - A];

  // calculates the final position in the array.
  --count[0]; // In c the first index is zero not 1.
  for (std::size_t i = 1; i < count_size; ++i)
    count[i] += count[i - 1];

  typedef typename std::iterator_traits<Iter>::value_type value_type;
  std::vector<value_type> out(N, 0);
  for (std::size_t i = 0; i < N; ++i) {
    const auto idx = begin[i] - A; // Index in the count array.
    const auto pos = count[idx]; // Final position in output array.
    out[pos] = begin[i]; // Assigns to the output array.
    --count[idx];
  }

  std::copy(std::begin(out), std::end(out), begin);
}

