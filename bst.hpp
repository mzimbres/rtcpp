#include <vector>
#include <utility>
#include <iterator>

#include "inorder_iterator.hpp"
#include "node_pool.hpp"

template <typename T>
class bst {
  public:
  typedef T key_type;
  typedef node<T> node_type;
  typedef node_type* node_pointer;
  typedef inorder_iterator<T> const_iterator;
  typedef const_iterator iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  private:
  static const int rbit = 1;
  static const int lbit = 2;
  typedef std::vector<node_type> pool_type;
  typedef typename pool_type::size_type size_type;
  node_pool<T> pool;
  node_type head;
  node_pointer attach_node_right(node_pointer p);
  node_pointer attach_node_left(node_pointer p);
  bst(const bst& rhs); // To be implemented
  node_pointer inorder_successor(node_pointer p) const;
  node_pointer inorder_predecessor(node_pointer p) const;
  node_pointer preorder_successor(node_pointer p) const;
  public:
  void copy(bst& rhs); // Copies this to rhs.
  bst(std::size_t reserve_n);
  std::pair<iterator, bool> insert(T key);
  const_iterator begin() const;
  const_iterator end() const {return const_iterator(&head);}
  const_reverse_iterator rbegin() const {return const_reverse_iterator(end());}
  const_reverse_iterator rend() const {return const_reverse_iterator(begin());}
};

template <typename T>
void bst<T>::copy(bst<T>& rhs)
{
  if (this == &rhs)
    return;

  node_pointer p = &head;
  node_pointer q = &rhs.head;

  for (;;) {
    if (!(p->tag & lbit)) {
      node_pointer pair = rhs.attach_node_left(q);
      if (!pair)
        break;
    }

    p = preorder_successor(p);
    q = rhs.preorder_successor(q);

    if (p == &head)
      break;

    if (!(p->tag & rbit)) {
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
  while (!(q->tag & lbit))
    q = q->llink;

  return const_iter(q);
}

template <typename T>
typename bst<T>::node_pointer bst<T>::inorder_successor(node_pointer p) const
{
  if (p->tag & rbit)
    return p->rlink;

  node_pointer q = p->rlink;
  while (!(q->tag & lbit))
    q = q->llink;

  return q;
}

template <typename T>
typename bst<T>::node_pointer bst<T>::inorder_predecessor(node_pointer p) const
{
  if (p->tag & lbit)
    return p->llink;

  node_pointer q = p->llink;
  while (!(q->tag & rbit))
    q = q->rlink;

  return q;
}

template <typename T>
typename bst<T>::node_pointer bst<T>::preorder_successor(node_pointer p) const
{
  if (!(p->tag & lbit))
    return p->llink;

  if (!(p->tag & rbit))
    return p->llink;

  // This is a leaf node.
  return p->rlink->rlink;
}

template <typename T>
bst<T>::bst(std::size_t reserve_n)
: pool(reserve_n)
{
  head.llink = &head;
  head.rlink = &head;
  head.tag = lbit;
}

template <typename T>
typename bst<T>::node_pointer bst<T>::attach_node_right(node_pointer p)
{
  node_pointer q = pool.allocate();
  if (!q)
    return 0; // The tree has exhausted its capacity.

  q->rlink = p->rlink;
  q->tag = (q->tag & lbit) | (p->tag & rbit);
  p->rlink = q;
  p->tag = p->tag & lbit;
  q->llink = p;
  q->tag = q->tag | lbit;

  if (!(q->tag & rbit)) {
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
  q->tag = (q->tag & rbit) | (p->tag & lbit);
  p->llink = q;
  p->tag = p->tag & rbit;
  q->rlink = p;
  q->tag = q->tag | rbit;

  if (!(q->tag & lbit)) {
    node_pointer qs = inorder_predecessor(q);
    qs->rlink = q;
  }

  return q;
}

template <typename T>
std::pair<typename bst<T>::iterator, bool> bst<T>::insert(T key)
{
  if (head.tag & lbit) { // The tree is empty
    node_pointer p = attach_node_left(&head);
    if (p)
      p->key = key;
    return std::make_pair(const_iterator(p), static_cast<bool>(p));
  }

  node_pointer p = head.llink;
  for (;;) {
    if (key < p->key) {
      if (!(p->tag & lbit)) {
        p = p->llink;
        continue;
      }
      node_pointer pair = attach_node_left(p);
      if (pair)
        pair->key = key;
      return std::make_pair(pair, static_cast<bool>(pair));
    } else if (key > p->key) {
      if (!(p->tag & rbit)) {
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

