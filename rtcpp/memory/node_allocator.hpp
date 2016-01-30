#pragma once

#include <array>
#include <vector>
#include <utility>
#include <memory>
#include <exception>
#include <type_traits>

#include <rtcpp/memory/node_stack.hpp>

  /*

  Implementation of a node allocator.  It performs constant
  time allocation on a pre-allocated buffer.

  Upon construction, only pointers are stored and you cannot
  use its member functions. They can be used only after
  copy-constructing the rebound instance, which will happen
  inside the container. At that point we know the size of
  the type the allocator will serve and can link together
  blocks of that size on the buffer.

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
    using other = node_allocator< U
                                , sizeof (U)
                                , !(sizeof (U) < sizeof (char*))>;
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
    using other = node_allocator< U
                                , sizeof (U)
                                , !(sizeof (U) < sizeof (char*))>;
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
struct allocator_traits<rt::node_allocator<T>> {
  using use_node_allocation = typename rt::node_allocator<T>::use_node_allocation;
  using is_always_equal = std::false_type;
  using allocator_type = typename rt::node_allocator<T>;
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
void swap(rt::node_allocator<T>& s1, rt::node_allocator<T>& s2)
{
  s1.swap(s2);
}

}

