#pragma once

#include <iterator>

namespace rtcpp {

template <std::size_t S>
struct alloc_block { // Check if S is a multiple of word size.
  alloc_block* llink;
  char padding[S - 1];
};

template <std::size_t S>
class node_stack {
  public:
  typedef alloc_block<S> node_type;
  typedef node_type* pointer;
  typedef const node_type* const_pointer;
  private:
  pointer avail;
  public:
  node_stack(pointer p) noexcept
  : avail(p)
  {}
  node_stack() noexcept : avail(0) {}
  pointer pop() noexcept;
  void push(pointer p) noexcept;
};

template <std::size_t S>
typename node_stack<S>::pointer node_stack<S>::pop() noexcept
{
  pointer q = avail;
  if (avail)
    avail = avail->llink;
  return q;
}

template <std::size_t S>
void node_stack<S>::push(typename node_stack<S>::pointer p) noexcept
{
  if (!p)
    return;

  //p->rlink = 0;
  p->llink = avail;
  avail = p;
}

}

