#pragma once

#include <iterator>

namespace rt {

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
  pointer avail;
  public:
  node_stack(void* p, std::size_t n) noexcept;
  node_stack() noexcept : avail(0) {}
  pointer pop() noexcept;
  void push(pointer p) noexcept;
};

template <std::size_t S>
node_stack<S>::node_stack(void* p, std::size_t n) noexcept
{
  pointer begin = reinterpret_cast<pointer>(p);
  std::size_t m = n / S; // Number of alloc_blocks we need.
  avail = link_stack(begin, begin + m);
}

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

