#pragma once

#include <array>
#include <utility>
#include <exception>

#include <rtcpp/memory/node_stack.hpp>

  /*

  Implementation of a node allocator.  It performs constant time allocation on
  a pre-allocated buffer.

  Upon construction, only pointers are stored and you cannot use its member
  functions. They can be used only after copy-constructing the rebound
  instance, which will happen inside the container. At that point we know the
  size of the type the allocator will serve and can link together blocks of
  that size on the buffer.

  */

namespace rt {

template < typename T
         , std::size_t S = sizeof (T)
         , bool B = !(S < sizeof (char*))
         >
class node_allocator {
  public:
  char* m_data;
  std::size_t m_size;
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
    typedef node_allocator< U
                          , sizeof (U)
                          , !(sizeof (U) < sizeof (char*))> other;
  };
  bool operator==(const node_allocator& alloc) const
  {return m_data == alloc.m_data;}
  bool operator!=(const node_allocator& alloc) const
  {return !(*this == alloc);}
  void swap(node_allocator& other) noexcept
  {
    std::swap(m_size, other.m_size);
    std::swap(m_data, other.m_data);
  }
  node_allocator(char* data, std::size_t size)
  : m_data(data)
  , m_size(size)
  {}
  template <std::size_t N>
  explicit node_allocator(std::array<char, N>& arr)
  : node_allocator(&arr.front(), arr.size())
  {}
  explicit node_allocator(std::vector<char>& arr)
  : node_allocator(&arr.front(), arr.size())
  {}
  template<typename U>
  node_allocator(const node_allocator< U
                                     , sizeof (U)
                                     , !(sizeof (U) < sizeof (char*))>& alloc)
  : m_data(alloc.m_data)
  , m_size(alloc.m_size)
  {}
};

template <typename T, std::size_t N>
class node_allocator<T, N, true> {
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
    typedef node_allocator< U
                          , sizeof (U)
                          , !(sizeof (U) < sizeof (char*))> other;
  };
  public:
  char* m_data;
  std::size_t m_size;
  node_stack<sizeof (T)> m_stack;
  public:
  node_allocator(char* data, std::size_t size)
  : m_data(data)
  , m_size(size)
  {}
  // Stack is not linked in this ctor
  template <std::size_t I>
  explicit node_allocator(std::array<char, I>& arr)
  : node_allocator(&arr.front(), arr.size())
  {}
  // Stack is not linked in this ctor
  explicit node_allocator(std::vector<char>& arr)
  : node_allocator(&arr.front(), arr.size())
  {}
  template<typename U>
  node_allocator(const node_allocator< U
                                     , sizeof (U)
                                     , !(sizeof (U) < sizeof (char*))>& alloc)
  : m_data(alloc.m_data)
  , m_size(alloc.m_size)
  , m_stack(m_data, m_size)
  {}
  pointer allocate(size_type)
  {
    char* p = m_stack.pop(); 
    if (!p)
      throw std::bad_alloc();
    return reinterpret_cast<pointer>(p); 
  }
  void deallocate(pointer p, size_type)
  { m_stack.push(reinterpret_cast<char*>(p)); }
  template<typename U>
  void destroy(U* p) {p->~U();}
  template< typename U, typename... Args>
  void construct(U* p, Args&&... args)
  {::new((void *)p) U(std::forward<Args>(args)...);}
  bool operator==(const node_allocator& alloc) const
  {return m_stack == alloc.m_stack;}
  bool operator!=(const node_allocator& alloc) const
  {return !(*this == alloc);}
  void swap(node_allocator& other) noexcept
  {
    m_stack.swap(other.m_stack);
    std::swap(m_data, other.m_data);
    std::swap(m_size, other.m_size);
  }
  pointer address(reference x) const noexcept { return std::addressof(x); }
  const_pointer address(const_reference x) const noexcept
  { return std::addressof(x); }
};

}

namespace std {

template <typename T>
void swap(rt::node_allocator<T>& s1, rt::node_allocator<T>& s2)
{
  s1.swap(s2);
}

}

