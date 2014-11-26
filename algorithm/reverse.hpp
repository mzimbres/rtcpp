#pragma once

#include <iterator>

namespace rtcpp {

// For random access iterators.

template<typename Iter>
void reverse(Iter begin, Iter end)
{
  while (begin < end) {
    std::iter_swap(begin, --end);
    ++begin;
  }
}
        
}

