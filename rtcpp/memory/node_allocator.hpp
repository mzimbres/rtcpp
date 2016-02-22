#pragma once

#include <array>
#include <vector>
#include <utility>
#include <memory>
#include <exception>
#include <type_traits>

#include "node_stack.hpp"
#include "node_traits.hpp"

/*
  This is the prototype allocator I have implemented for the proposal.

  Please, read the proposal in doc/proposal_allocator.pdf
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
  {}
  template <std::size_t I>
  explicit node_allocator(std::array<char, I>& arr)
  : node_allocator(&arr.front(), arr.size())
  {}
  template <typename Alloc>
  explicit node_allocator(std::vector<char, Alloc>& arr)
  : node_allocator(&arr.front(), arr.size())
  {}
  // Constructor for the node type with a different pointer type.
  template<typename U, typename K = T>
  node_allocator( const node_allocator<U, NodeType>& alloc
                , typename std::enable_if< is_same_node_type<K, NodeType>::value>::type p = 0)
  : m_data(alloc.m_data)
  , m_size(alloc.m_size)
  , m_stack(m_data, m_size, sizeof (T))
  {}
  template<typename U>
  node_allocator(const node_allocator<U, NodeType>& alloc)
  : m_data(alloc.m_data)
  , m_size(alloc.m_size)
  {}
  node_allocator(const node_allocator& alloc)
  : m_data(alloc.m_data)
  , m_size(alloc.m_size)
  {}
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

