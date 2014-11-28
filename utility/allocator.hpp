#pragma once

#include <utility/node_stack.hpp>

namespace rtcpp {

template <typename T, bool B = (sizeof (T) > sizeof (char*))>
class allocator {
  public:
  std::size_t m_size;
  void* m_data;
  public:
  typedef T value_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  template< class U >
  struct rebind { typedef allocator<U, (sizeof (U) > sizeof (char*))> other; };
  allocator()
  : m_size(0)
  , m_data(0)
  {}
  allocator(void* data, std::size_t size)
  : m_size(size)
  , m_data(data)
  {}
};

template <typename T>
class allocator<T, true> {
  public:
  typedef T value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  template<class U>
  struct rebind { typedef allocator<U, (sizeof (U) > sizeof (char*))> other;};
  private:
  node_stack<sizeof(value_type)> m_stack;
  node_stack<sizeof(value_type)>* m_stack_pointer;
  public:
  // Should be used only once. How to determine this at compile time?
  template<typename U>
  allocator(const allocator<U, false>& alloc)
  : m_stack(alloc.m_data, alloc.m_size)
  , m_stack_pointer(&m_stack)
  {}
  template<typename U>
  allocator(const allocator<U, true>& alloc)
  : m_stack_pointer(alloc.m_stack_pointer)
  {}
  pointer allocate(size_type) { return reinterpret_cast<pointer>(m_stack_pointer->pop()); }
  void deallocate(pointer p, size_type) { m_stack_pointer->push(reinterpret_cast<alloc_block<sizeof(value_type)>*>(p)); }
};

}

