#pragma once

namespace rt
{

// Faster than std::find.
// Assumes the last element is set to the searched value.
// std::distance(begin, last) is assumed to be at least 1.

template <typename Iter, typename T>
Iter find_intrusive(Iter begin, Iter end, const T& v)
{
  if (begin == --end) // Empty range.
    return ++end;

  while (*begin != v)
    ++begin;

  return begin;
}

}

