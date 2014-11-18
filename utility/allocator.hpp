#pragma once

#include <utility/node_stack.hpp>

namespace rtcpp {

template <typename T, bool B = (sizeof (T) > sizeof (char*))>
class allocator {
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
  //pointer allocate(size_type) {return new T;}
  //void deallocate(pointer p, size_type) {delete p;}
  allocator() {}
  allocator(const allocator&) {}
  template<class U>
  allocator(const allocator<U, false>&);
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
  public:
  allocator() {}
  allocator(void* p, std::size_t n)
  : m_stack(p, n)
  {}
  pointer allocate(size_type) { return reinterpret_cast<pointer>(m_stack.pop()); }
  void deallocate(pointer p, size_type) { m_stack.push(reinterpret_cast<alloc_block<sizeof(value_type)>*>(p)); }
};

}

