#pragma once

#include <iterator>
#include <utility>
#include <cstring>
#include <exception>
#include <array>
#include <algorithm>

#include "link_stack.hpp"
#include "align.hpp"

namespace rt {

class node_stack {
  // I do not know whether the standard guarantees the
  // condition below, since I rely on it I will test it.
  static_assert( !((sizeof (std::uintptr_t)) > (sizeof (char*)))
               , "node_stack: Unable to use this class in this platform.");
  private:
  static const std::size_t ptr_size = sizeof (char*);
  static const std::size_t counter_offset = 0;
  static const std::size_t avail_offset = ptr_size;
  static const std::size_t node_size_offset = 2 * ptr_size;
  static const std::size_t pool_offset = counter_offset
                                       + avail_offset 
                                       + node_size_offset;
  char* m_data;
  // used only when default constructed. Will be soon removed.
  std::array<char, pool_offset + ptr_size> m_dummy_buffer;
  char* get_counter_ptr() const noexcept {return m_data + counter_offset;}
  char* get_avail_ptr() const noexcept {return m_data + avail_offset;}
  char* get_node_size_ptr() const noexcept {return m_data + node_size_offset;}
  char* get_pool_ptr() const noexcept {return m_data + pool_offset;}
  public:
  node_stack();
  node_stack(char* p, std::size_t n, std::size_t S);
  char* pop() noexcept;
  void push(char* p) noexcept;
  bool operator==(const node_stack& rhs) const noexcept {return m_data == rhs.m_data;}
  void swap(node_stack& other) noexcept {std::swap(m_data, other.m_data);}
};

// Move this to cpp file.
node_stack::node_stack()
{
  std::fill(std::begin(m_dummy_buffer), std::end(m_dummy_buffer), 0);
  m_data = &m_dummy_buffer[0];
  std::size_t size = m_dummy_buffer.size();
  align_if_needed<ptr_size>(m_data, size);
}

// Move this to cpp file.
node_stack::node_stack(char* p, std::size_t n, std::size_t S)
: m_data(p)
{
  // p is expected to be (sizeof pointer) aligned and its
  // memory zero-initialized.  The first word pointed to by
  // p will be used to store a counter of how many times
  // this constructor has been called. The second word, a
  // pointer to the avail stack and the third the number of
  // bytes in each node.  That way we can know whether the
  // same allocator instance is being used to serve
  // containers with nodes of different size. We can report
  // an error this way.

  align_if_needed<ptr_size>(m_data, n);
  const std::size_t min_size = pool_offset + 2 * S;
  if (n < min_size)
    throw std::runtime_error("node_stack: There is not enough space.");

  std::uintptr_t counter; // Current counter value
  std::memcpy(&counter, get_counter_ptr(), ptr_size);

  if (counter != 0) { // Links only once.
    std::uintptr_t node_size; // Current node size in the buffer.
    std::memcpy(&node_size, get_node_size_ptr(), ptr_size);
    if (node_size < S)
      throw std::runtime_error("node_stack: Avail stack already linked for node with incompatible size.");
  } else { // Links only once.
    char* top = link_stack(get_pool_ptr(), n - pool_offset, S);
    std::memcpy(get_avail_ptr(), &top, ptr_size);
    const std::uintptr_t node_size = S;
    std::memcpy(get_node_size_ptr(), &node_size, ptr_size);
  }
  ++counter;
  std::memcpy(m_data, &counter, ptr_size);
}

inline
char* node_stack::pop() noexcept
{
  char* q;
  std::memcpy(&q, get_avail_ptr(), ptr_size);
  if (q)
    std::memcpy(get_avail_ptr(), q, ptr_size);

  return q;
}

inline
void node_stack::push(char* p) noexcept
{
  std::memcpy(p, get_avail_ptr(), ptr_size);
  std::memcpy(get_avail_ptr(), &p, ptr_size);
}

}

