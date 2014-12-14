#pragma once

#include <array>

#include <memory/node_stack.hpp>

#include "align.hpp"

namespace rt {

template < typename T
         , std::size_t S = size_of<T>::size
         , bool B = !(S < size_of<char*>::size)
         >
class allocator {
  private:
  allocator(const allocator* rhs);
  const allocator& operator=(const allocator& rhs);
  public:
  std::size_t m_size;
  char* m_data;
  public:
  typedef T value_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  template<typename U>
  struct rebind {
    typedef allocator< U
                     , size_of<U>::size
                     , !(size_of<U>::size < size_of<char*>::size)> other;
  };
  void init(char* data, std::size_t size)
  {
    m_size = size;
    m_data = data;
    // aligns the pointer on a word boundary.
    const std::size_t a = reinterpret_cast<std::size_t>(m_data);
    const std::size_t b = sizeof (char*); // word size.
    const std::size_t c = is_aligned(a, b) ? a : next_aligned(a, b);
    m_data = reinterpret_cast<char*>(c);
    size = c - a;
  }
  allocator(char* data, std::size_t size) { init(data, size); }
  template <std::size_t N = 200>
  allocator(const std::array<char, N>& arr = std::array<char,N>())
  {
    init(const_cast<char*>(&arr.front()), arr.size());
  }
};

template <typename T, std::size_t N>
class allocator<T, N, true> {
  public:
  typedef T value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  template<class U>
  struct rebind {
    typedef allocator< U
                     , size_of<U>::size
                     , !(size_of<U>::size < size_of<char*>::size)> other;
  };
  private:
  node_stack<size_of<value_type>::size> m_stack;
  node_stack<size_of<value_type>::size>* m_stack_pointer;
  public:
  template<typename U>
  allocator(const allocator<U, size_of<U>::size, false>& alloc)
  : m_stack(alloc.m_data, alloc.m_size)
  , m_stack_pointer(&m_stack)
  {}
  template<typename U>
  allocator(const allocator<U, size_of<U>::size, true>& alloc)
  : m_stack_pointer(alloc.m_stack_pointer)
  {}
  pointer allocate(size_type) { return reinterpret_cast<pointer>(m_stack_pointer->pop()); }
  void deallocate(pointer p, size_type) { m_stack_pointer->push(reinterpret_cast<char*>(p)); }
  template<typename U>
  void destroy(U* p) {p->~U();}
  template< typename U, typename... Args>
  void construct(U* p, Args&&... args) {::new((void *)p) U(std::forward<Args>(args)...);}
};

}

