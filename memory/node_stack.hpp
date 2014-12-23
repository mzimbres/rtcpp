#pragma once

#include <iterator>
#include <utility>
#include <cstring>

namespace rt {

template <std::size_t S> // Block size in bytes.
char* link_stack(char* p, std::size_t n)
{
  // n: Number of bytes beginning at p.

  // Number of blocks of size S we have available.
  const std::size_t m = n / S;

  // The minimum number of blocks we need is 2.
  if (m < 2)
    return 0;

  const std::size_t ptr_size = sizeof (char*);
  for (std::size_t i = 1; i < m; ++i) {
    char* pp = p + (i - 1) * S; // Pointer to the previous block.
    char* pn = p + i * S; // Pointer to the next block.
    // We now store the address of the previous block in the memory
    // location of the begin of the next block.
    std::memcpy(pn, &pp, ptr_size);
  }

  std::memset(p, 0, ptr_size);
  return p + (m - 1) * S; // Pointer to the top of the stack.
}

template <std::size_t S>
class node_stack {
  private:
  std::size_t* m_counter;
  char** m_avail;
  public:
  node_stack(char* p, std::size_t n) noexcept;
  node_stack() noexcept {}
  char* pop() noexcept;
  void push(char* p) noexcept;
  bool operator==(const node_stack& rhs) const {return m_counter == rhs.m_counter;}
  void swap(node_stack& other) noexcept;
};

template <std::size_t S>
void node_stack<S>::swap(node_stack<S>& other) noexcept
{
  std::swap(m_counter, other.m_counter);
  std::swap(m_avail, other.m_avail);
}

template <std::size_t S>
node_stack<S>::node_stack(char* p, std::size_t n) noexcept
{
  // p is expected to be word aligned and its memory zero-initialized.
  // The first word pointed to by p will store a counter of how many times the
  // stack has been linked. The second, a pointer to the avail stack.
  // TODO: check whether n is big enough.

  const std::size_t s = sizeof (char*);
  m_counter = &reinterpret_cast<std::size_t&>(*p);
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

