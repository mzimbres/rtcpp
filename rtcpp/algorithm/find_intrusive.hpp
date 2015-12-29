#pragma once

#include <iterator>

namespace rt
{

// Faster than std::find.
// Assumes the last element is set to the searched value.
// std::distance(begin, last) is assumed to be at least 1.

template <typename Iter, typename T>
Iter find_intrusive(Iter begin, Iter end, const T& v)
{
  auto data_end = std::prev(end);
  if (begin == data_end) // Empty range.
    return end;

  while (*begin != v)
    ++begin;

  if (begin == data_end)
    return end;

  return begin;
}

}

