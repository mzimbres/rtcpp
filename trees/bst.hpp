#include <vector>
#include <utility>
#include <iterator>
#include <functional>

#include "bst_iterator.hpp"
#include "node_pool.hpp"

namespace rtcpp {

template <typename T, typename Compare = std::less<T>>
class bst {
  public:
  typedef T key_type;
  typedef bst_iterator<T> const_iterator;
  typedef const_iterator iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  private:
  typedef node<T> node_type;
  typedef node_type* node_pointer;
  typedef const node_type* const_node_pointer;
  typedef std::vector<node_type> pool_type;
  typedef typename pool_type::size_type size_type;
  node_pool<T> pool;
  node_type head;
  Compare comp;
  bst(const bst& rhs); // To be implemented
  public:
  void copy(bst& rhs) const; // Copies this to rhs.
  bst(std::size_t reserve_n);
  ~bst();
  void clear();
  std::pair<iterator, bool> insert(T key);
  const_iterator begin() const {return const_iterator(inorder_successor(&head));}
  const_iterator end() const {return const_iterator(&head);}
  const_reverse_iterator rbegin() const {return const_reverse_iterator(end());}
  const_reverse_iterator rend() const {return const_reverse_iterator(begin());}
};

template <typename T, typename Compare>
bst<T, Compare>::bst(std::size_t reserve_n)
: pool(reserve_n)
{
  head.llink = &head;
  head.rlink = &head;
  head.tag = detail::lbit;
}

template <typename T, typename Compare>
void bst<T, Compare>::clear()
{
}

template <typename T, typename Compare>
bst<T, Compare>::~bst()
{
  clear();
}

template <typename T, typename Compare>
void bst<T, Compare>::copy(bst<T, Compare>& rhs) const
{
  if (this == &rhs)
    return;

  const_node_pointer p = &head;
  node_pointer q = &rhs.head;

  for (;;) {
    if (!has_null_llink(p->tag)) {
      node_pointer tmp = rhs.pool.allocate();
      if (!tmp)
        break; // The tree has exhausted its capacity.

      attach_node_left(q, tmp);
    }

    p = preorder_successor(p);
    q = preorder_successor(q);

    if (p == &head)
      break;

    if (!has_null_rlink(p->tag)) {
      node_pointer tmp = rhs.pool.allocate();
      if (!tmp)
        break; // The tree has exhausted its capacity.

      attach_node_right(q, tmp);
    }

    q->key = p->key;
  }
}

template <typename T, typename Compare>
std::pair<typename bst<T, Compare>::iterator, bool> bst<T, Compare>::insert(T key)
{
  typedef typename bst<T>::const_iterator const_iterator;
  if (has_null_llink(head.tag)) { // The tree is empty
    node_pointer q = pool.allocate();
    if (!q)
      return std::make_pair(const_iterator(), false); // The tree has exhausted its capacity.

    attach_node_left(&head, q);
    q->key = key;
    return std::make_pair(const_iterator(q), true);
  }

  node_pointer p = head.llink;
  for (;;) {
    if (comp(key, p->key)) {
      if (!has_null_llink(p->tag)) {
        p = p->llink;
        continue;
      }
      node_pointer q = pool.allocate();
      if (!q)
        return std::make_pair(const_iterator(), false); // The tree has exhausted its capacity.

      attach_node_left(p, q);
      q->key = key;
      return std::make_pair(q, true);
    } else if (comp(p->key, key)) {
      if (!has_null_rlink(p->tag)) {
        p = p->rlink;
        continue;
      }
      node_pointer q = pool.allocate();
      if (!q)
        return std::make_pair(const_iterator(), false); // The tree has exhausted its capacity.

      attach_node_right(p, q);
      q->key = key;
      return std::make_pair(q, true);
    } else {
      return std::make_pair(p, false);
    }
  }
}

}
