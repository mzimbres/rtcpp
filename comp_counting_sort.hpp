#pragma once

#include <iterator>
#include <functional>
#include <algorithm>
#include <array>

template <unsigned N, typename Iter>
bool comp_counting_sort(Iter begin)
{
  typedef typename std::iterator_traits<Iter>::value_type value_type;
  unsigned buffer[N] = {0};

  for (int i = N - 1; i > 0; --i)
    for (int j = i - 1; j >= 0; --j)
      if (begin[i] < begin[j])
        ++buffer[j];
      else
        ++buffer[i];

  value_type buffer2[N];
  for (unsigned i = 0; i < N; ++i)
    buffer2[buffer[i]] = begin[i];

  std::copy(&buffer2[0], &buffer2[0] + N, begin);
}

