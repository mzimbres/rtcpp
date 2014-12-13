#pragma once

#include <iterator>

namespace rt {

template <typename ForwardIter>
typename std::iterator_traits<ForwardIter>::pointer link_stack(ForwardIter begin, std::size_t size)
{
  if (size < 2)
    return 0;

  begin->llink = 0;
  for (std::size_t i = 1; i < size; ++i)
    begin[i].llink = &begin[i - 1];

  return &begin[size - 1]; // Pointer to the top of the stack.
}

template <std::size_t S>
struct alloc_block { // Check if S is a multiple of word size.
  alloc_block* llink;
  char padding[S - sizeof(alloc_block*)];
};

template <std::size_t S>
class node_stack {
  public:
  typedef alloc_block<S> node_type;
  typedef node_type* pointer;
  typedef const node_type* const_pointer;
  private:
  node_type* tmp; // We have to put this pointer in the buffer.
  node_type** avail;
  public:
  node_stack(void* p, std::size_t n) noexcept;
  node_stack() noexcept : tmp(0) {}
  pointer pop() noexcept;
  void push(pointer p) noexcept;
};

template <std::size_t S>
node_stack<S>::node_stack(void* p, std::size_t n) noexcept
{
  pointer begin = reinterpret_cast<pointer>(p);
  std::size_t m = n / S; // Number of alloc_blocks we need.
  tmp = link_stack(begin, m);
  avail = &tmp;
}

template <std::size_t S>
typename node_stack<S>::pointer node_stack<S>::pop() noexcept
{
  pointer q = *avail;
  if (*avail)
    *avail = (*avail)->llink;
  return q;
}

template <std::size_t S>
void node_stack<S>::push(typename node_stack<S>::pointer p) noexcept
{
  if (!p)
    return;

  p->llink = *avail;
  *avail = p;
}

}

