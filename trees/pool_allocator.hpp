#pragma once

#include "bst_node.hpp"
#include "node_stack.hpp"

namespace rtcpp {

template <typename T>
class pool_allocator {
  public:
  typedef T value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  pointer allocate() const noexcept
  {
    try {
      return new value_type;
    } catch (const std::bad_alloc& e) {
      return 0;
    }
  }
  void deallocate(pointer p) const noexcept {delete p;};

  template <typename U>
  struct rebind {
    typedef pool_allocator<U> other;
  };
};

template <typename T>
class pool_allocator<bst_node<T>> {
  public:
  typedef bst_node<T> node_type;
  typedef node_type value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  private:
  node_stack<node_type>* m_pool;
  public:
  pool_allocator(node_stack<node_type>* p) noexcept : m_pool(p) {}
  pool_allocator() noexcept : m_pool(0) {}
  pointer allocate() noexcept {return m_pool->pop();}
  void deallocate(pointer p) noexcept {m_pool->push(p);};

  template <typename U>
  struct rebind {
    typedef pool_allocator<U> other;
  };
};

}

