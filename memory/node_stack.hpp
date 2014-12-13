#pragma once

#include <iterator>

namespace rt {

template <std::size_t S> // Blocks size.
char* link_stack(char* p, std::size_t n)
{
  // n: Number of bytes beginning at p.

  const std::size_t m = n / S; // Number of blocks of size S we need.

  if (m < 2) // Minimum number of block we need.
    return 0;

  for (std::size_t i = 1; i < m; ++i) {
    char* pn = p + i * S; // Points to the begin of the second block.
    char& ad = *pn; // Memory address where pn points to.
    // Pretends that ad is the address of a char* and not a char.
    char*& pad = reinterpret_cast<char*&>(ad);
    // sets the value of that pointer to the previous block.
    pad = &*(p + (i - 1) * S);
  }

  reinterpret_cast<char*&>(*p) = 0;
  return p + (m - 1) * S; // Pointer to the top of the stack.
}

template <std::size_t S>
class node_stack {
  private:
  char* tmp; // We have to put this pointer in the buffer.
  char** avail;
  public:
  node_stack(char* p, std::size_t n) noexcept;
  node_stack() noexcept : tmp(0) {}
  char* pop() noexcept;
  void push(char* p) noexcept;
};

template <std::size_t S>
node_stack<S>::node_stack(char* p, std::size_t n) noexcept
{
  tmp = link_stack<S>(p, n);
  avail = &tmp;
}

template <std::size_t S>
char* node_stack<S>::pop() noexcept
{
  char* q = *avail;
  if (q)
    *avail = reinterpret_cast<char*&>(**avail);
  return q;
}

template <std::size_t S>
void node_stack<S>::push(char* p) noexcept
{
  if (!p)
    return;

  reinterpret_cast<char*&>(*p) = *avail;
  *avail = p;
}

}

