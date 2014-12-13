#include <initializer_list>
#include <utility>
#include <iterator>
#include <functional>
#include <algorithm>
#include <memory>
#include <limits>

#include <memory/allocator.hpp>

#include "bst_iterator.hpp"

namespace rt {

template < typename T
         , typename Compare = std::less<T>
         , typename Allocator = allocator<T>>
class bst { // Unbalanced binary search tree
  public:
  typedef bst_node<T> node_type; // The standard does not require this to be public.
  typedef T key_type;
  typedef T value_type;
  typedef std::size_t size_type;
  typedef Compare key_compare;
  typedef Compare value_compare;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef typename std::allocator_traits<Allocator>::pointer pointer;
  typedef typename std::allocator_traits<Allocator>::const_pointer const_pointer;
  typedef std::ptrdiff_t difference_type;
  typedef bst_iterator<T> const_iterator;
  typedef const_iterator iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  private:
  typedef typename std::allocator_traits<Allocator>::template rebind_alloc<node_type> inner_allocator_type;
  typedef node_type* node_pointer;
  typedef const node_type* const_node_pointer;
  inner_allocator_type m_inner_alloc;
  node_type head;
  Compare m_comp;
  void copy(bst& rhs) const noexcept;
  public:
  bst(const Compare& comp, const Allocator& alloc) noexcept;
  explicit bst(const Allocator& alloc) noexcept
  : bst(Compare(), alloc) {}
  bst(const bst& rhs) noexcept;
  bst& operator=(const bst& rhs) noexcept;
  bst& operator=(std::initializer_list<T> init) noexcept;
  template <typename InputIt>
  bst(InputIt begin, InputIt end, const Compare& comp, const Allocator& alloc) noexcept;
  template <typename InputIt>
  bst(InputIt begin, InputIt end, const Allocator& alloc) noexcept
  : bst(begin, end, Compare(), alloc) {}
  bst(std::initializer_list<T> init, const Compare& comp, const Allocator& alloc) noexcept
  : bst(std::begin(init), std::end(init), comp, alloc) {}
  bst(std::initializer_list<T> init, const Allocator& alloc) noexcept
  : bst(init, Compare(), alloc) {}
  ~bst() noexcept;
  void clear() noexcept;
  std::pair<iterator, bool> insert(const value_type& key) noexcept;
  const_iterator begin() const noexcept {return const_iterator(inorder_successor(&head));}
  const_iterator end() const noexcept {return const_iterator(&head);}
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
  const_iterator find(const K& x) const noexcept;
  size_type max_size() const noexcept{ return std::numeric_limits<size_type>::max(); }
  template<typename InputIt>
  void insert(InputIt begin, InputIt end) noexcept;
};

template <typename T, typename Compare, typename Allocator>
bst<T, Compare, Allocator>& bst<T, Compare, Allocator>::operator=(const bst<T, Compare, Allocator>& rhs) noexcept
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
bst<T, Compare, Allocator>& bst<T, Compare, Allocator>::operator=(std::initializer_list<T> init) noexcept
{
  clear();
  insert(std::begin(init), std::end(init));
  return *this;
}

template <typename T, typename Compare, typename Allocator>
bst<T, Compare, Allocator>::bst(const bst<T, Compare, Allocator>& rhs) noexcept
: m_inner_alloc(std::allocator_traits<inner_allocator_type>::select_on_container_copy_construction(rhs.m_inner_alloc))
{
  // This ctor can fail if the allocator runs out of memory.
  head.llink = &head;
  head.rlink = &head;
  head.tag = detail::lbit;
  clear();
  rhs.copy(*this);
}

template <typename T, typename Compare, typename Allocator>
bst<T, Compare, Allocator>::bst(const Compare& comp, const Allocator& alloc) noexcept
: m_inner_alloc(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc))
, m_comp(comp)
{
  head.llink = &head;
  head.rlink = &head;
  head.tag = detail::lbit;
}

template <typename T, typename Compare, typename Allocator>
template <typename InputIt>
bst<T, Compare, Allocator>::bst(InputIt begin, InputIt end, const Compare& comp, const Allocator& alloc) noexcept
: m_inner_alloc(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc))
, m_comp(comp)
{
  head.llink = &head;
  head.rlink = &head;
  head.tag = detail::lbit;
  auto func = [this](const T& tmp) -> void {
    auto pair = this->insert(tmp);
    if (pair.second)
      return;
  };
  std::for_each(begin, end, func);
}

template <typename T, typename Compare, typename Allocator>
void bst<T, Compare, Allocator>::clear() noexcept
{
  node_pointer p = &head;
  for (;;) {
    node_pointer q = inorder_successor(p);
    if (p != &head) {
      std::allocator_traits<inner_allocator_type>::destroy(m_inner_alloc, &q->key);
      std::allocator_traits<inner_allocator_type>::deallocate(m_inner_alloc, p, 1);
    }
    if (q == &head)
      break;
    p = q;
  }
  head.llink = &head;
  head.rlink = &head;
  head.tag = detail::lbit;
}

template <typename T, typename Compare, typename Allocator>
bst<T, Compare, Allocator>::~bst() noexcept
{
  clear();
}

template <typename T, typename Compare, typename Allocator>
void bst<T, Compare, Allocator>::copy(bst<T, Compare, Allocator>& rhs) const noexcept
{
  const_node_pointer p = &head;
  node_pointer q = &rhs.head;

  for (;;) {
    if (!has_null_llink(p->tag)) {
      node_pointer tmp = std::allocator_traits<inner_allocator_type>::allocate(rhs.m_inner_alloc, 1);
      if (!tmp)
        break; // The tree has exhausted its capacity.

      attach_node_left(q, tmp);
    }

    p = preorder_successor(p);
    q = preorder_successor(q);

    if (p == &head)
      break;

    if (!has_null_rlink(p->tag)) {
      node_pointer tmp = std::allocator_traits<inner_allocator_type>::allocate(rhs.m_inner_alloc, 1);
      if (!tmp)
        break; // The tree has exhausted its capacity.

      attach_node_right(q, tmp);
    }

    q->key = p->key;
  }
}

template <typename T, typename Compare, typename Allocator>
std::pair<typename bst<T, Compare, Allocator>::iterator, bool>
bst<T, Compare, Allocator>::insert(const typename bst<T, Compare, Allocator>::value_type& key) noexcept
{
  typedef typename bst<T>::const_iterator const_iterator;
  if (has_null_llink(head.tag)) { // The tree is empty
    node_pointer q = std::allocator_traits<inner_allocator_type>::allocate(m_inner_alloc, 1);
    if (!q)
      return std::make_pair(const_iterator(), false); // The tree has exhausted its capacity.

    attach_node_left(&head, q);
    std::allocator_traits<inner_allocator_type>::construct(m_inner_alloc, std::addressof(q->key), key);
    return std::make_pair(const_iterator(q), true);
  }

  node_pointer p = head.llink;
  for (;;) {
    if (m_comp(key, p->key)) {
      if (!has_null_llink(p->tag)) {
        p = p->llink;
        continue;
      }
      node_pointer q = std::allocator_traits<inner_allocator_type>::allocate(m_inner_alloc, 1);
      if (!q)
        return std::make_pair(const_iterator(), false); // The tree has exhausted its capacity.

      attach_node_left(p, q);
      std::allocator_traits<inner_allocator_type>::construct(m_inner_alloc, std::addressof(q->key), key);
      return std::make_pair(q, true);
    } else if (m_comp(p->key, key)) {
      if (!has_null_rlink(p->tag)) {
        p = p->rlink;
        continue;
      }
      node_pointer q = std::allocator_traits<inner_allocator_type>::allocate(m_inner_alloc, 1);
      if (!q)
        return std::make_pair(const_iterator(), false); // The tree has exhausted its capacity.

      attach_node_right(p, q);
      std::allocator_traits<inner_allocator_type>::construct(m_inner_alloc, std::addressof(q->key), key);
      return std::make_pair(q, true);
    } else {
      return std::make_pair(p, false);
    }
  }
}

template <typename T, typename Compare, typename Allocator>
template <typename K>
typename bst<T, Compare, Allocator>::size_type
bst<T, Compare, Allocator>::count(const K& key) const noexcept
{
  typedef typename bst<T>::const_iterator const_iterator;
  if (has_null_llink(head.tag)) // The tree is empty
    return 0;

  node_pointer p = head.llink;
  for (;;) {
    if (m_comp(key, p->key)) {
      if (!has_null_llink(p->tag)) {
        p = p->llink;
        continue;
      }
      return 0;
    } else if (m_comp(p->key, key)) {
      if (!has_null_rlink(p->tag)) {
        p = p->rlink;
        continue;
      }
      return 0;
    } else {
      return 1;
    }
  }
}

template <typename T, typename Compare, typename Allocator>
template <typename K>
typename bst<T, Compare, Allocator>::const_iterator
bst<T, Compare, Allocator>::find(const K& key) const noexcept
{
  typedef typename bst<T>::const_iterator const_iterator;
  if (has_null_llink(head.tag)) // The tree is empty
    return const_iterator(&head); // end iterator.

  node_pointer p = head.llink;
  for (;;) {
    if (m_comp(key, p->key)) {
      if (!has_null_llink(p->tag)) {
        p = p->llink;
        continue;
      }
      return const_iterator(&head); // end iterator.
    } else if (m_comp(p->key, key)) {
      if (!has_null_rlink(p->tag)) {
        p = p->rlink;
        continue;
      }
      return const_iterator(&head); // end iterator.
    } else {
      return p; // equivalent element.
    }
  }
}

template <typename T, typename Compare, typename Allocator>
template <typename InputIt>
void bst<T, Compare, Allocator>::insert(InputIt begin, InputIt end) noexcept
{
  for (InputIt iter = begin; iter != end; ++iter) {
    auto pair = insert(*iter);
    if (!pair.second)
       continue;
  }
}

template<typename Key, typename Compare, typename Alloc>
bool operator==( const bst<Key, Compare, Alloc>& lhs
               , const bst<Key, Compare, Alloc>& rhs) noexcept
{
  const bool b1 = lhs.size() == rhs.size();
  const bool b2 = std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs));
  if (b1 && b2)
    return true;

  return false;
}

template<typename Key, typename Compare, typename Alloc>
bool operator!=( const bst<Key, Compare, Alloc>& lhs
               , const bst<Key, Compare, Alloc>& rhs) noexcept
{
  return !(lhs == rhs);
}

}

