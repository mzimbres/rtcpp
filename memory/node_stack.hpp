#pragma once

#include <iterator>
#include <utility>
#include <cstring>
#include <exception>

#include "link_stack.hpp"

namespace rt {

template <std::size_t S>
class node_stack {
  // I do not know whether the standard guarantees the condition below, so I am
  // testing it.
  static_assert(!((sizeof (std::uintptr_t)) > (sizeof (char*))), "node_stack: Unable to use this class in this platform.");
  private:
  static const std::size_t ptr_size = sizeof (char*);
  static const std::size_t counter_offset = 0;
  static const std::size_t avail_offset = ptr_size;
  static const std::size_t node_size_offset = 2 * ptr_size;
  static const std::size_t pool_offset = counter_offset + avail_offset + node_size_offset;
  char* m_data;
  char* get_counter_ptr() const {return m_data + counter_offset;}
  char* get_avail_ptr() const {return m_data + avail_offset;}
  char* get_node_size_ptr() const {return m_data + node_size_offset;}
  char* get_pool_ptr() const {return m_data + pool_offset;}
  public:
  node_stack(char* p, std::size_t n);
  node_stack() noexcept {}
  char* pop() noexcept;
  void push(char* p) noexcept;
  bool operator==(const node_stack& rhs) const {return m_data == rhs.m_data;}
  void swap(node_stack& other) noexcept {std::swap(m_data, other.m_data);}
};

template <std::size_t S>
node_stack<S>::node_stack(char* p, std::size_t n)
: m_data(p)
{
  // p is expected to be (sizeof pointer) aligned and its memory zero-initialized.
  // The first word pointed to by p will be used to store a counter of how many times the
  // stack has been linked. The second, a pointer to the avail stack and the third
  // the number of bytes in ech node. Tht way we can know whether the same allocator
  // instance is being used to serve containers with nodes of different size.

  const std::size_t min_size = pool_offset + 2 * S;
  if (n < min_size)
    throw std::runtime_error("node_stack: There is not enough space.");

  std::uintptr_t counter; // Current value of the counter.
  std::memcpy(&counter, get_counter_ptr(), ptr_size);

  if (counter != 0) { // Links only once.
    std::uintptr_t node_size;
    std::memcpy(&node_size, get_node_size_ptr(), ptr_size);
    if (node_size != S)
      throw std::runtime_error("node_stack: Avail stack already linked for node with different size.");
  } else { // Links only once.
    // The first word will be used to store a pointer to the avail node.
    char* top = link_stack<S>(get_pool_ptr(), n - pool_offset);
    std::memcpy(get_avail_ptr(), &top, ptr_size);
    const std::uintptr_t node_size = S;
    std::memcpy(get_node_size_ptr(), &node_size, ptr_size);
  }
  ++counter;
  std::memcpy(m_data, &counter, ptr_size);
}

template <std::size_t S>
char* node_stack<S>::pop() noexcept
{
  char* q;
  std::memcpy(&q, get_avail_ptr(), ptr_size);
  if (q)
    std::memcpy(get_avail_ptr(), q, ptr_size);

  return q;
}

template <std::size_t S>
void node_stack<S>::push(char* p) noexcept
{
  if (!p)
    return;

  std::memcpy(p, get_avail_ptr(), ptr_size);
  std::memcpy(get_avail_ptr(), &p, ptr_size);
}

}

