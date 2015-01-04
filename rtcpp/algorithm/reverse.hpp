#pragma once

#include <iterator>

namespace rt {

// For random access iterators.

template<typename Iter>
void reverse(Iter begin, Iter end) noexcept
{
  while (begin < end) {
    std::iter_swap(begin, --end);
    ++begin;
  }
}
        
}

