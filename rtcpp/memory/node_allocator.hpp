#pragma once

#include <array>
#include <vector>
#include <utility>
#include <memory>
#include <exception>
#include <type_traits>

#include <rtcpp/memory/node_stack.hpp>

/*
  This is the prototype allocator I have implemented for the proposal.

  Please, read the proposal in doc/proposal_allocator.pdf

  Performs constant time allocation on a pre-allocated buffer.

  Node allocation can happen only when the node size is greater than
  or equal to the size of a pointer, otherwise it is not possible to
  link nodes together.  Therefore this class has a specialization for
  such types.
*/

namespace rt {

template <typename T, typename NodeType>
class node_allocator {
  static_assert( ((sizeof (NodeType)) >= (sizeof (char*)))
               , "node_allocator: incompatible node size.");
  public:
  static constexpr std::size_t memory_use = node_stack::memory_use;
  using use_node_allocation = std::true_type;
  using pointer = T*;
  using const_pointer = const T*;
  using const_reference = const T&;
  using reference = T&;
  static constexpr std::size_t S = sizeof (NodeType);
  template<class U>
  struct rebind { using other = node_allocator<U , NodeType>; };
  public:
  char* m_data;
  std::size_t m_size;
  node_stack m_stack;
  public:
  node_allocator(char* data, std::size_t size)
  : m_data(data)
  , m_size(size)
  , m_stack(m_data, m_size, S)
  {}
  template <std::size_t I>
  explicit node_allocator(std::array<char, I>& arr)
  : node_allocator(&arr.front(), arr.size())
  {}
  template <typename Alloc>
  explicit node_allocator(std::vector<char, Alloc>& arr)
  : node_allocator(&arr.front(), arr.size())
  {}
  // Copy constructor, always tries to link the stack. If it is
  // already linked ok. If it is linked to an incompatible size,
  // compile time error.
  template<typename U, typename K>
  node_allocator(const node_allocator<U, K>& alloc)
  : m_data(alloc.m_data)
  , m_size(alloc.m_size)
  , m_stack(m_data, m_size, S)
  {
    static_assert( ((sizeof (NodeType)) >= (sizeof (K)))
                 , "node_allocator: incompatible node size.");
  }
  // allocate_node is only enabled to if the allocator value_type
  // is equal to the node_type.
  template <typename U = T>
  typename std::enable_if<std::is_same<U, NodeType>::value, pointer>::type
  allocate_node()
  {
    char* p = m_stack.pop(); 
    if (!p)
      throw std::bad_alloc();
    return reinterpret_cast<pointer>(p); 
  }
  template <typename U = T>
  typename std::enable_if<std::is_same<U, NodeType>::value>::type
  deallocate_node(pointer p)
  { m_stack.push(reinterpret_cast<char*>(p)); }
  template<typename U>
  void destroy(U* p) {p->~U();}
  template< typename U, typename... Args>
  void construct(U* p, Args&&... args)
  {::new((void *)p) U(std::forward<Args>(args)...);}
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

template <typename T, typename K, typename U, typename V>
bool operator==( const node_allocator<T, K>& alloc1
               , const node_allocator<U, V>& alloc2)
{return alloc1.m_stack == alloc2.m_stack;}

template <typename T, typename K, typename U, typename V>
bool operator!=( const node_allocator<T, K>& alloc1
               , const node_allocator<U, V>& alloc2)
{return !(alloc1 == alloc2);}

}

namespace std {

template <typename T, typename NodeType>
struct allocator_traits<rt::node_allocator<T, NodeType>> {
  using use_node_allocation = typename rt::node_allocator<T, NodeType>::use_node_allocation;
  using is_always_equal = std::false_type;
  using allocator_type = typename rt::node_allocator<T, NodeType>;
  using const_reference = const T&;
  using pointer = T*;
  using size_type = std::size_t;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using const_void_pointer = typename std::pointer_traits<pointer>::template rebind<const void>;
  using void_pointer = typename std::pointer_traits<pointer>::template rebind<void>;
  using const_pointer = typename allocator_type::const_pointer;
  using propagate_on_container_copy_assignment = std::true_type;
  using propagate_on_container_move_assignment = std::true_type;
  using propagate_on_container_swap = std::true_type;
  template<typename U>
  using rebind_alloc =
    typename allocator_type::template rebind<U>::other;
  static allocator_type
    select_on_container_copy_construction(const allocator_type& a)
    {return a;}
  static pointer allocate_node(allocator_type& a)
  {return a.allocate_node();}
  static void deallocate_node( allocator_type& a
                        , pointer p) {a.deallocate_node(p);}
  template<class U>
  static void destroy(allocator_type& a, U* p) {a.destroy(p);}
  template<class U, class... Args >
  static void construct(allocator_type& a, U* p, Args&&... args)
  {a.construct(p, std::forward<Args>(args)...);}
};

template <typename T, typename K, typename U, typename V>
void swap(rt::node_allocator<T, K>& s1, rt::node_allocator<U, V>& s2)
{
  // Put some static assert here.
  s1.swap(s2);
}

}

