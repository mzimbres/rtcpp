#pragma once

#include <array>
#include <vector>
#include <utility>
#include <memory>
#include <exception>
#include <type_traits>

#include <rtcpp/memory/node_stack.hpp>

/*

  Implementation of a node allocator.  It performs constant time
  allocation on a pre-allocated buffer.

  Node allocation can happen only when the node size is greater than
  or equal to the size of a pointer, otherwise it is not possible to
  link nodes together.  Therefore this class has a specialization for
  such types.

  This allocator is lazy, that means, when it is constructed
  only the pointer to the buffer and the size is stored. Only on 
  copy construction this buffer is divided in blocks and linked
  together. Copy construction occurrs inside the container with
  the new rebound allocator type.

  I also offer a constructor to divide and link the buffer
  immediately, but currently there is no way of knowing the right
  size, since this is only known inside the container.

  In general it is possible to delay linking the buffer until the
  rebound type is copy constructed. However, on unordered containers
  the allocator is rebound twice and both are copy constructed,
  therefore, in this case, I do have to inform the size.

*/

namespace rt {

// TODO: Add constructors that link the buffer.
template < typename T
         , typename NodeType = T
         , std::size_t S = sizeof (T)
         , bool B = !(S < sizeof (char*))
         >
class node_allocator_lazy {
  public:
  char* m_data;
  std::size_t m_size;
  public:
  static constexpr std::size_t memory_use = node_stack::memory_use;
  using use_node_allocation = std::true_type;
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  template<typename U>
  struct rebind {
    using other = node_allocator_lazy< U
                                     , NodeType
                                     , sizeof (U)
                                     , !(sizeof (U) < sizeof (char*))>;
  };
  bool operator==(const node_allocator_lazy& alloc) const
  {return m_data == alloc.m_data;}
  bool operator!=(const node_allocator_lazy& alloc) const
  {return !(*this == alloc);}
  void swap(node_allocator_lazy& other) noexcept
  {
    std::swap(m_size, other.m_size);
    std::swap(m_data, other.m_data);
  }
  node_allocator_lazy(char* data, std::size_t size)
  : m_data(data)
  , m_size(size)
  {}
  template <std::size_t N>
  explicit node_allocator_lazy(std::array<char, N>& arr)
  : node_allocator_lazy(&arr.front(), arr.size())
  {}
  explicit node_allocator_lazy(std::vector<char>& arr)
  : node_allocator_lazy(&arr.front(), arr.size())
  {}
  template<typename U, typename K>
  node_allocator_lazy(const node_allocator_lazy< U
                                               , K
                                               , sizeof (U)
                                               , !(sizeof (U) < sizeof (char*))>& alloc)
  : m_data(alloc.m_data)
  , m_size(alloc.m_size)
  {}
};

template < typename T
         , typename NodeType
         , std::size_t N>
class node_allocator_lazy<T, NodeType, N, true> {
  public:
  using use_node_allocation = std::true_type;
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  template<class U>
  struct rebind {
    using other = node_allocator_lazy< U
                                     , NodeType
                                     , sizeof (U)
                                     , !(sizeof (U) < sizeof (char*))>;
  };
  public:
  char* m_data;
  std::size_t m_size;
  node_stack m_stack;
  public:
  // The following ctors only store the pointer and the size.  Use
  // them if you want linking to occurr inside the container.
  node_allocator_lazy(char* data, std::size_t size)
  : m_data(data)
  , m_size(size)
  {}
  template <std::size_t I>
  explicit node_allocator_lazy(std::array<char, I>& arr)
  : node_allocator_lazy(&arr.front(), arr.size())
  {}
  template <typename Alloc>
  explicit node_allocator_lazy(std::vector<char, Alloc>& arr)
  : node_allocator_lazy(&arr.front(), arr.size())
  {}
  // The following ctors store the pointer, the size and do link the
  // buffer. Use them if you do not wnt this to happen inside the
  // container.
  node_allocator_lazy(char* data, std::size_t size, std::size_t S)
  : m_data(data)
  , m_size(size)
  , m_stack(m_data, m_size, S)
  {}
  template <std::size_t I>
  explicit node_allocator_lazy(std::array<char, I>& arr, std::size_t S)
  : node_allocator_lazy(&arr.front(), arr.size(), S)
  {}
  template <typename Alloc>
  explicit node_allocator_lazy(std::vector<char, Alloc>& arr, std::size_t S)
  : node_allocator_lazy(&arr.front(), arr.size(), S)
  {}
  // Copy constructor, always tries to link the stack. If it is already
  // linked ok. If it is linked to an incompatible size, throws.
  template<typename U, typename K>
  node_allocator_lazy(const node_allocator_lazy< U
                                               , K
                                               , sizeof (U)
                                               , !(sizeof (U) < sizeof (char*))>& alloc)
  : m_data(alloc.m_data)
  , m_size(alloc.m_size)
  , m_stack(m_data, m_size, N)
  {}
  pointer allocate_node()
  {
    char* p = m_stack.pop(); 
    if (!p)
      throw std::bad_alloc();
    return reinterpret_cast<pointer>(p); 
  }
  pointer allocate(size_type)
  {
    return allocate_node();
  }
  void deallocate_node(pointer p)
  { m_stack.push(reinterpret_cast<char*>(p)); }
  void deallocate(pointer p, size_type)
  { deallocate_node(p); }
  template<typename U>
  void destroy(U* p) {p->~U();}
  template< typename U, typename... Args>
  void construct(U* p, Args&&... args)
  {::new((void *)p) U(std::forward<Args>(args)...);}
  bool operator==(const node_allocator_lazy& alloc) const
  {return m_stack == alloc.m_stack;}
  bool operator!=(const node_allocator_lazy& alloc) const
  {return !(*this == alloc);}
  void swap(node_allocator_lazy& other) noexcept
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
struct allocator_traits<rt::node_allocator_lazy<T>> {
  using use_node_allocation = typename rt::node_allocator_lazy<T>::use_node_allocation;
  using is_always_equal = std::false_type;
  using allocator_type = typename rt::node_allocator_lazy<T>;
  using size_type = typename allocator_type::size_type;
  using pointer = typename allocator_type::pointer;
  using value_type = typename allocator_type::value_type;
  using difference_type = typename allocator_type::difference_type;
  using const_pointer = typename allocator_type::const_pointer;
  using propagate_on_container_copy_assignment = std::true_type;
  using propagate_on_container_move_assignment = std::true_type;
  using void_pointer = typename std::pointer_traits<pointer>::template rebind<void>;
  using const_void_pointer = typename std::pointer_traits<pointer>::template rebind<const void>;
  using propagate_on_container_swap = std::true_type;
  template<typename U>
  using rebind_alloc =
    typename allocator_type::template rebind<U>::other;
  static allocator_type
    select_on_container_copy_construction(const allocator_type& a)
    {return a;}
  static pointer allocate(allocator_type& a, size_type)
  {return a.allocate_node();}
  static pointer allocate_node(allocator_type& a)
  {return a.allocate_node();}
  static void deallocate( allocator_type& a
                        , pointer p
                        , size_type) {a.deallocate_node(p);}
  static void deallocate_node( allocator_type& a
                        , pointer p) {a.deallocate_node(p);}
  template<class U>
  static void destroy(allocator_type& a, U* p) {a.destroy(p);}
  template<class U, class... Args >
  static void construct(allocator_type& a, U* p, Args&&... args)
  {a.construct(p, std::forward<Args>(args)...);}
};

template <typename T>
void swap(rt::node_allocator_lazy<T>& s1, rt::node_allocator_lazy<T>& s2)
{
  s1.swap(s2);
}

}

