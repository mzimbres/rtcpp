#include <vector>
#include <utility>
#include <iterator>

#include "bst_iterator.hpp"
#include "node_pool.hpp"

namespace rtcpp {

template <typename T>
class bst {
  public:
  typedef T key_type;
  typedef node<T> node_type;
  typedef node_type* node_pointer;
  typedef bst_iterator<T> const_iterator;
  typedef const_iterator iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  private:
  typedef std::vector<node_type> pool_type;
  typedef typename pool_type::size_type size_type;
  node_pool<T> pool;
  node_type head;
  node_pointer attach_node_right(node_pointer p);
  node_pointer attach_node_left(node_pointer p);
  bst(const bst& rhs); // To be implemented
  public:
  void copy(bst& rhs); // Copies this to rhs.
  bst(std::size_t reserve_n);
  ~bst();
  void clear();
  std::pair<iterator, bool> insert(T key);
  const_iterator begin() const;
  const_iterator end() const {return const_iterator(&head);}
  const_reverse_iterator rbegin() const {return const_reverse_iterator(end());}
  const_reverse_iterator rend() const {return const_reverse_iterator(begin());}
};

template <typename T>
void bst<T>::clear()
{
}

template <typename T>
bst<T>::~bst<T>()
{
  clear();
}

template <typename T>
void bst<T>::copy(bst<T>& rhs)
{
  if (this == &rhs)
    return;

  node_pointer p = &head;
  node_pointer q = &rhs.head;

  for (;;) {
    if (!has_null_llink(p->tag)) {
      node_pointer pair = rhs.attach_node_left(q);
      if (!pair)
        break;
    }

    p = preorder_successor(p);
    q = preorder_successor(q);

    if (p == &head)
      break;

    if (!has_null_rlink(p->tag)) {
      node_pointer pair = rhs.attach_node_right(q);
      if (!pair)
        break;
    }

    q->key = p->key;
  }
}

template <typename T>
typename bst<T>::const_iterator bst<T>::begin() const
{
  typedef typename bst<T>::const_iterator const_iter;
  node_pointer q = head.llink;
  while (!has_null_llink(q->tag))
    q = q->llink;

  return const_iter(q);
}

template <typename T>
bst<T>::bst(std::size_t reserve_n)
: pool(reserve_n)
{
  head.llink = &head;
  head.rlink = &head;
  head.tag = detail::lbit;
}

template <typename T>
typename bst<T>::node_pointer bst<T>::attach_node_right(node_pointer p)
{
  node_pointer q = pool.allocate();
  if (!q)
    return 0; // The tree has exhausted its capacity.

  q->rlink = p->rlink;
  q->tag = has_null_llink(q->tag) | has_null_rlink(p->tag);
  p->rlink = q;
  p->tag = has_null_llink(p->tag);
  q->llink = p;
  q->tag = set_lbit(q->tag);

  if (!has_null_rlink(q->tag)) {
    node_pointer qs = inorder_successor(q);
    qs->llink = q;
  }

  return q;
}

template <typename T>
typename bst<T>::node_pointer bst<T>::attach_node_left(node_pointer p)
{
  node_pointer q = pool.allocate();
  if (!q)
    return 0; // The tree has exhausted its capacity.

  q->llink = p->llink;
  q->tag = has_null_rlink(q->tag) | has_null_llink(p->tag);
  p->llink = q;
  p->tag = has_null_rlink(p->tag);
  q->rlink = p;
  q->tag = set_rbit(q->tag);

  if (!has_null_llink(q->tag)) {
    node_pointer qs = inorder_predecessor(q);
    qs->rlink = q;
  }

  return q;
}

template <typename T>
std::pair<typename bst<T>::iterator, bool> bst<T>::insert(T key)
{
  if (has_null_llink(head.tag)) { // The tree is empty
    node_pointer p = attach_node_left(&head);
    if (p)
      p->key = key;
    return std::make_pair(const_iterator(p), static_cast<bool>(p));
  }

  node_pointer p = head.llink;
  for (;;) {
    if (key < p->key) {
      if (!has_null_llink(p->tag)) {
        p = p->llink;
        continue;
      }
      node_pointer pair = attach_node_left(p);
      if (pair)
        pair->key = key;
      return std::make_pair(pair, static_cast<bool>(pair));
    } else if (key > p->key) {
      if (!has_null_rlink(p->tag)) {
        p = p->rlink;
        continue;
      }
      node_pointer q = attach_node_right(p);
      if (q)
        q->key = key;
      return std::make_pair(const_iterator(q), static_cast<bool>(q));
    } else {
      return std::make_pair(p, false);
    }
  }
}

}

