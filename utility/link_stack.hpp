#pragma once

#include <iterator>

namespace rtcpp {

template <typename ForwardIter>
typename std::iterator_traits<ForwardIter>::pointer link_stack(ForwardIter begin, ForwardIter end)
{
  if (std::distance(begin, end) < 2)
    return 0;

  begin->llink = 0;
  ForwardIter iter = begin;
  ForwardIter next = ++begin;
  while (next != end) {
    next->llink = &*iter;
    ++iter;
    ++next;
  }

  return &*iter; // Pointer to the top of the stack.
}

}

