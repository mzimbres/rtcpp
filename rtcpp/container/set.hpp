#include <memory>
#include <limits>
#include <utility>
#include <iterator>
#include <algorithm>
#include <functional>
#include <type_traits>
#include <initializer_list>

#include "bst_iterator.hpp"

/*
  Implements an std::set as a threaded binary search tree. That means it does
  not guarantee logarithmic search time. It is however often faster than a
  balanced implementation as a degenerate tree is very rare and there is no
  balancing overhead.

  NEWS: It supports allocators that can serve only one object at a time:
  allocator_type::allocate();
*/

namespace rt {

template < typename T
         , typename Compare = std::less<T>
         , typename Allocator = std::allocator<T>>
class set {
  public:
  using key_type = T;
  using value_type = T;
  using size_type = std::size_t ;
  using key_compare = Compare;
  using value_compare = Compare;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
  using difference_type = std::ptrdiff_t;
  using const_iterator = bst_iterator<T>;
  using iterator = const_iterator;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  private:
  using node_type = bst_node<T>;
  using inner_allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<node_type>;
  using node_pointer = typename inner_allocator_type::pointer;
  using const_node_pointer = const node_type*;
  mutable inner_allocator_type m_inner_alloc;
  node_pointer m_head;
  Compare m_comp;
  void copy(set& rhs) const noexcept;
  node_pointer get_node() const;
  void safe_construct(node_pointer p, const value_type& key) const;
  public:
  static std::size_t reserve_to_alloc(std::size_t n) {return sizeof ((node_type) + 1) * n;}
  set(const Compare& comp, const Allocator& alloc = Allocator());
  explicit set(const Allocator& alloc = Allocator())
  : set(Compare(), alloc) {}
  set(const set& rhs) noexcept;
  set& operator=(const set& rhs) noexcept;
  set& operator=(std::initializer_list<T> init) noexcept;
  template <typename InputIt>
  set(InputIt begin, InputIt end, const Compare& comp, const Allocator& alloc = Allocator());
  template <typename InputIt>
  set(InputIt begin, InputIt end, const Allocator& alloc = Allocator())
  : set(begin, end, Compare(), alloc) {}
  set(std::initializer_list<T> init, const Compare& comp, const Allocator& alloc = Allocator())
  : set(std::begin(init), std::end(init), comp, alloc) {}
  set(std::initializer_list<T> init, const Allocator& alloc = Allocator())
  : set(init, Compare(), alloc) {}
  set(set&& rhs);
  ~set() noexcept;
  void clear() noexcept;
  std::pair<iterator, bool> insert(const value_type& key) noexcept;
  const_iterator begin() const noexcept {return const_iterator(inorder<1>(m_head));}
  const_iterator end() const noexcept {return const_iterator(m_head);}
  const_reverse_iterator rbegin() const noexcept {return const_reverse_iterator(end());}
  const_reverse_iterator rend() const noexcept {return const_reverse_iterator(begin());}
  key_compare key_comp() const noexcept {return m_comp;}
  value_compare value_comp() const noexcept {return m_comp;}
  size_type size() const noexcept {return std::distance(begin(), end());}
  bool empty() const noexcept {return begin() == end();}
  Allocator get_allocator() const noexcept {return m_inner_alloc;}
  template<typename K>
  size_type count(const K& x) const noexcept;
  template<typename K>
  const_iterator find(const K& x) const;
  template<typename K>
  std::pair<const_node_pointer, const_node_pointer> find_parent(const K& x) const;
  size_type max_size() const noexcept { return std::numeric_limits<size_type>::max(); }
  template<typename InputIt>
  void insert(InputIt begin, InputIt end) noexcept;
  void swap(set& other) noexcept;
  template <typename K>
  size_type erase(const K& key);
};

template <typename T, typename Compare, typename Allocator>
set<T, Compare, Allocator>::set(set<T, Compare, Allocator>&& rhs)
: m_inner_alloc(rhs.m_inner_alloc)
, m_head(get_node())
, m_comp(std::move(rhs.m_comp))
{
  m_head->link[0] = m_head;
  m_head->link[1] = m_head;
  m_head->tag = detail::lbit;
  std::swap(m_inner_alloc, rhs.m_inner_alloc);
  std::swap(m_head, rhs.m_head);
}

template <typename T, typename Compare, typename Allocator>
template <typename K>
typename set<T, Compare, Allocator>::size_type
set<T, Compare, Allocator>::erase(const K& key)
{
  auto pair = find_parent(key);
  const_node_pointer q = pair.first;
  const_node_pointer pq = pair.second;
  if (q == m_head)
    return 0;

  node_pointer r = erase_node<1>(const_cast<node_pointer>(pq), const_cast<node_pointer>(q));
  std::allocator_traits<inner_allocator_type>::deallocate(m_inner_alloc, r, 1);
  return 1;
}

template <typename T, typename Compare, typename Allocator>
void set<T, Compare, Allocator>::swap(set<T, Compare, Allocator>& other) noexcept
{
  std::swap(m_inner_alloc, other.m_inner_alloc); // this one is in namespace rt.
  std::swap(m_head, other.m_head);
  std::swap(m_comp, other.m_comp);
}

template <typename T, typename Compare, typename Allocator>
set<T, Compare, Allocator>& set<T, Compare, Allocator>::operator=(const set<T, Compare, Allocator>& rhs) noexcept
{
  // This ctor can fail if the allocator runs out of memory.
  if (this == &rhs)
    return *this;

  clear();
  if (std::allocator_traits<Allocator>::propagate_on_container_copy_assignment::value)
    m_inner_alloc = rhs.m_inner_alloc;

  rhs.copy(*this);

  return *this;
}

template <typename T, typename Compare, typename Allocator>
set<T, Compare, Allocator>& set<T, Compare, Allocator>::operator=(std::initializer_list<T> init) noexcept
{
  clear();
  insert(std::begin(init), std::end(init));
  return *this;
}

template <typename T, typename Compare, typename Allocator>
set<T, Compare, Allocator>::set(const set<T, Compare, Allocator>& rhs) noexcept
: m_inner_alloc(std::allocator_traits<inner_allocator_type>::select_on_container_copy_construction(rhs.m_inner_alloc))
, m_head(get_node())
{
  m_head->link[0] = m_head;
  m_head->link[1] = m_head;
  m_head->tag = detail::lbit;
  clear();
  rhs.copy(*this);
}

template <typename T, typename Compare, typename Allocator>
set<T, Compare, Allocator>::set(const Compare& comp, const Allocator& alloc)
: m_inner_alloc(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc))
, m_head(get_node())
, m_comp(comp)
{
  m_head->link[0] = m_head;
  m_head->link[1] = m_head;
  m_head->tag = detail::lbit;
}

template <typename T, typename Compare, typename Allocator>
template <typename InputIt>
set<T, Compare, Allocator>::set(InputIt begin, InputIt end, const Compare& comp, const Allocator& alloc)
: m_inner_alloc(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc))
, m_head(get_node())
, m_comp(comp)
{
  m_head->link[0] = m_head;
  m_head->link[1] = m_head;
  m_head->tag = detail::lbit;
  insert(begin, end);
}

template <typename T, typename Compare, typename Allocator>
void set<T, Compare, Allocator>::clear() noexcept
{
  node_pointer p = m_head;
  for (;;) {
    node_pointer q = inorder<1>(p);
    if (p != m_head) {
      std::allocator_traits<inner_allocator_type>::destroy(m_inner_alloc, &q->key);
      std::allocator_traits<inner_allocator_type>::deallocate(m_inner_alloc, p, 1);
    }
    if (q == m_head)
      break;
    p = q;
  }
  m_head->link[0] = m_head;
  m_head->link[1] = m_head;
  m_head->tag = detail::lbit;
}

template <typename T, typename Compare, typename Allocator>
set<T, Compare, Allocator>::~set() noexcept
{
  clear();
  std::allocator_traits<inner_allocator_type>::deallocate(m_inner_alloc, m_head, 1);
}

template <typename T, typename Compare, typename Allocator>
void set<T, Compare, Allocator>::copy(set<T, Compare, Allocator>& rhs) const noexcept
{
  const_node_pointer p = m_head;
  node_pointer q = rhs.m_head;

  for (;;) {
    if (!has_null_link<0>::apply(p)) {
      node_pointer tmp = get_node();
      attach_node<0>(q, tmp);
    }

    p = preorder_successor(p);
    q = preorder_successor(q);

    if (p == m_head)
      break;

    if (!has_null_link<1>::apply(p)) {
      node_pointer tmp = get_node();
      attach_node<1>(q, tmp);
    }

    q->key = p->key;
  }
}

namespace detail
{

template<class T, class R = void>  
struct enable_if_type { using type = R; };

template<class T, class Enable = void>
struct test_use_node_alloc : std::false_type {};

template<class T>
struct test_use_node_alloc< T
                          , typename enable_if_type<typename T::use_node_alloc>::type
                          > : std::true_type {};


template <typename Alloc>
typename
std::enable_if< detail::test_use_node_alloc<std::allocator_traits<Alloc>>::value
              , typename std::allocator_traits<Alloc>::pointer>::type
get_node_free(Alloc& alloc)
{
  return std::allocator_traits<Alloc>::allocate_node(alloc);
}

template <typename Alloc>
typename
std::enable_if< !detail::test_use_node_alloc<std::allocator_traits<Alloc>>::value
              , typename std::allocator_traits<Alloc>::pointer>::type
get_node_free(Alloc& alloc)
{
  return std::allocator_traits<Alloc>::allocate(alloc, 1);
}

}

template <typename T, typename Compare, typename Allocator>
typename set<T, Compare, Allocator>::node_pointer set<T, Compare, Allocator>::get_node() const
{ 
  return detail::get_node_free(m_inner_alloc);
}

template <typename T, typename Compare, typename Allocator>
void
set<T, Compare, Allocator>::safe_construct( typename set<T, Compare, Allocator>::node_pointer p
                                          , const typename set<T, Compare, Allocator>::value_type& key) const
{
  try {
    std::allocator_traits<inner_allocator_type>::construct(m_inner_alloc, std::addressof(p->key), key);
  } catch (...) {
    std::allocator_traits<inner_allocator_type>::deallocate(m_inner_alloc, p, 1);
    throw;
  }
}

template <typename T, typename Compare, typename Allocator>
std::pair<typename set<T, Compare, Allocator>::iterator, bool>
set<T, Compare, Allocator>::insert(const typename set<T, Compare, Allocator>::value_type& key) noexcept
{
  if (has_null_link<0>::apply(m_head)) { // The tree is empty
    node_pointer q = get_node();
    safe_construct(q, key);
    attach_node<0>(m_head, q);
    return std::make_pair(const_iterator(q), true);
  }

  node_pointer p = m_head->link[0];
  for (;;) {
    if (m_comp(key, p->key)) {
      if (!has_null_link<0>::apply(p)) {
        p = p->link[0];
      } else {
        node_pointer q = get_node();
        safe_construct(q, key);
        attach_node<0>(p, q);
        return std::make_pair(q, true);
      }
    } else if (m_comp(p->key, key)) {
      if (!has_null_link<1>::apply(p)) {
        p = p->link[1];
      } else {
        node_pointer q = get_node();
        safe_construct(q, key);
        attach_node<1>(p, q);
        return std::make_pair(q, true);
      }
    } else {
      return std::make_pair(p, false);
    }
  }
}

template <typename T, typename Compare, typename Allocator>
template <typename K>
typename set<T, Compare, Allocator>::size_type
set<T, Compare, Allocator>::count(const K& key) const noexcept
{
  if (has_null_link<0>::apply(m_head)) // The tree is empty
    return 0;

  node_pointer p = m_head->link[0];
  for (;;) {
    if (m_comp(key, p->key)) {
      if (!has_null_link<0>::apply(p))
        p = p->link[0];
      else
        return 0;
    } else if (m_comp(p->key, key)) {
      if (!has_null_link<1>::apply(p))
        p = p->link[1];
      else
        return 0;
    } else {
      return 1;
    }
  }
}

template <typename T, typename Compare, typename Allocator>
template <typename K>
typename set<T, Compare, Allocator>::const_iterator
set<T, Compare, Allocator>::find(const K& key) const
{
  // The function below is not the most efficient because it
  // has an additional pointer to chase the parent pointer.
  // However maintaining two functions is not desirable
  auto p = find_parent(key);
  return const_iterator(p.first);
}

template <typename T, typename Compare, typename Allocator>
template <typename K>
std::pair< typename set<T, Compare, Allocator>::const_node_pointer
         , typename set<T, Compare, Allocator>::const_node_pointer>
set<T, Compare, Allocator>::find_parent(const K& key) const
{
  if (has_null_link<0>::apply(m_head)) // The tree is empty
    return std::make_pair(m_head, m_head); // end iterator.

  const_node_pointer u = m_head; // pointer to the parent pointer.
  const_node_pointer p = m_head->link[0];
  for (;;) {
    if (m_comp(key, p->key)) {
      if (!has_null_link<0>::apply(p)) {
        u = p;
        p = p->link[0];
      } else {
        return std::make_pair(m_head, m_head);
      }
    } else if (m_comp(p->key, key)) {
      if (!has_null_link<1>::apply(p)) {
        u = p;
        p = p->link[1];
      } else {
        return std::make_pair(m_head, m_head);
      }
    } else {
      return std::make_pair(p, u); // equivalent element.
    }
  }
}

template <typename T, typename Compare, typename Allocator>
template <typename InputIt>
void set<T, Compare, Allocator>::insert(InputIt begin, InputIt end) noexcept
{
  for (InputIt iter = begin; iter != end; ++iter) {
    auto pair = insert(*iter);
    if (!pair.second)
       continue;
  }
}

template<typename Key, typename Compare, typename Alloc>
bool operator==( const set<Key, Compare, Alloc>& lhs
               , const set<Key, Compare, Alloc>& rhs) noexcept
{
  const bool b1 = lhs.size() == rhs.size();
  const bool b2 = std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs));
  if (b1 && b2)
    return true;

  return false;
}

template<typename Key, typename Compare, typename Alloc>
bool operator!=( const set<Key, Compare, Alloc>& lhs
               , const set<Key, Compare, Alloc>& rhs) noexcept
{ return !(lhs == rhs); }

}

namespace std {

template <typename T>
void swap(rt::set<T>& s1, rt::set<T>& s2)
{ s1.swap(s2); }

}

