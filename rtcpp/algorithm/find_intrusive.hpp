#pragma once

namespace rt
{

// Faster than std::find.

template <typename Iter, typename T>
Iter find_intrusive(Iter begin, Iter end, const T& v)
{
  // I am assuming the last element in the range does not belong to the
  // set and is reserved to the algoithm. The range therefore contains at
  // least one element.

  --end;
  if (begin == end) // Empty range.
    return ++end;

  while (*begin != v)
    ++begin;

  return begin;
}

}

