#pragma once

#include "bst_node.hpp"
#include "node_stack.hpp"

namespace rtcpp {

template <typename T>
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
  struct rebind { typedef allocator<U> other; };
  pointer allocate(size_type, void*) {return new T;}
  void deallocate(pointer p, size_type) {delete p;}
  allocator() {}
  allocator(const allocator&) {}
  template<class U>
  allocator(const allocator<U>&);
};

template <typename T>
class allocator<bst_node<T>> {
  public:
  typedef bst_node<T> value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  template<class U>
  struct rebind { typedef allocator<U> other;};
  private:
  node_stack<value_type> m_stack;
  public:
  allocator() {}
  allocator(node_stack<value_type> stack)
  : m_stack(stack)
  {}
  pointer allocate(size_type, void*)
  {
    return m_stack.allocate(1);
  }
  void deallocate(pointer p, size_type)
  {
    m_stack.deallocate(p);
  }
};

}

