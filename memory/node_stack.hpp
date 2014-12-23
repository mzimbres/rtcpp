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
  std::size_t m_ptr_size;
  char* m_data;
  char* m_avail_ptr;
  public:
  node_stack(char* p, std::size_t n) noexcept;
  node_stack() noexcept {}
  char* pop() noexcept;
  void push(char* p) noexcept;
  bool operator==(const node_stack& rhs) const {return m_data == rhs.m_data;}
  void swap(node_stack& other) noexcept;
};

template <std::size_t S>
void node_stack<S>::swap(node_stack<S>& other) noexcept
{
  std::swap(m_ptr_size, other.m_ptr_size);
  std::swap(m_data, other.m_data);
  std::swap(m_avail_ptr, other.m_avail_ptr);
}

template <std::size_t S>
node_stack<S>::node_stack(char* p, std::size_t n) noexcept
: m_ptr_size(sizeof (char*))
, m_data(p)
, m_avail_ptr(m_data + m_ptr_size)
{
  // p is expected to be word aligned and its memory zero-initialized.
  // The first word pointed to by p will store a counter of how many times the
  // stack has been linked. The second, a pointer to the avail stack.
  // TODO: check whether n is big enough.

  // Current value of the counter.
  int counter = 0;
  std::memcpy(&counter, m_data, m_ptr_size);

  if (counter == 0) { // Links only once.
    // The first word will be used to store a pointer to the avail node.
    char* top = link_stack<S>(m_data + 2 * m_ptr_size, n - 2 * m_ptr_size);
    std::memcpy(m_avail_ptr, &top, m_ptr_size);
  }
  ++counter;
  const int int_size = sizeof (int);
  std::memcpy(m_data, &counter, int_size);
}

template <std::size_t S>
char* node_stack<S>::pop() noexcept
{
  char* q;
  std::memcpy(&q, m_avail_ptr, m_ptr_size);
  if (q)
    std::memcpy(m_avail_ptr, q, m_ptr_size);

  return q;
}

template <std::size_t S>
void node_stack<S>::push(char* p) noexcept
{
  if (!p)
    return;

  std::memcpy(p, m_avail_ptr, m_ptr_size);
  std::memcpy(m_avail_ptr, &p, m_ptr_size);
}

}

