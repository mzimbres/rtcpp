#pragma once

#include <iterator>

namespace rt {

template <std::size_t S> // Block size.
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
  int* m_counter;
  char** m_avail;
  public:
  node_stack(char* p, std::size_t n) noexcept;
  node_stack() noexcept {}
  char* pop() noexcept;
  void push(char* p) noexcept;
};

template <std::size_t S>
node_stack<S>::node_stack(char* p, std::size_t n) noexcept
{
  // p is expected to be word aligned and its memory zero-initialized.
  // The first word pointed to by p will store a counter of how many times the
  // stack has been linked. The second, a pointer to the avail stack.
  // TODO: check whether n is big enough.

  const std::size_t s = sizeof (char*);
  m_counter = &reinterpret_cast<int&>(*p);
  m_avail = &reinterpret_cast<char*&>(*(p + s));
  if (*m_counter == 0) { // Links only once.
    // The first word will be used to store a pointer to the avail node.
    *m_avail = link_stack<S>(p + 2 * s, n - 2 * s);
  }
  *m_counter += 1;
}

template <std::size_t S>
char* node_stack<S>::pop() noexcept
{
  char* q = *m_avail;
  if (q)
    *m_avail = reinterpret_cast<char*&>(**m_avail);
  return q;
}

template <std::size_t S>
void node_stack<S>::push(char* p) noexcept
{
  if (!p)
    return;

  reinterpret_cast<char*&>(*p) = *m_avail;
  *m_avail = p;
}

}

