#pragma once

namespace rt
{

template <typename Iter, typename T>
Iter find_fast(Iter begin, Iter end, const T& v)
{
  // I am assuming the last element in the range does not belong to the set and
  // is reserved to the algoithm. The range therefore contains at least one
  // element.

  *--end = v;
  if (begin == end) // Empty range.
    return ++end;

  while (*begin != v)
    ++begin;

  return begin;
}

}

