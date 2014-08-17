#include <iostream>
#include <vector>
#include <stack>

#include "bst_node.hpp"
#include "inorder_iterator.hpp"

template <typename T>
class bst {
  public:
  typedef node<T> node_type;
  typedef node_type* node_pointer;
  typedef inorder_iterator<T, node > const_iterator;
  private:
  typedef std::vector<node_type> pool_type;
  typedef typename pool_type::size_type size_type;
  std::vector<node_type> pool;
  node_type head;
  node_pointer avail;
  node_pointer insert_node_right(T key);
  node_pointer insert_node_left(T key);
  bst(const bst& rhs); // To be implemented
  node_pointer inorder_successor(node_pointer p) const;
  node_pointer inorder_predecessor(node_pointer p) const;
  const bst& operator=(const bst& rhs); // To be implemented
  public:
  bst(std::size_t reserve_n);
  node_pointer insert(T key);
  const_iterator begin() const;
  const_iterator end() const {return const_iterator(head);}
};

template <typename T>
bst<T>::const_iterator bst<T>::begin() const
{
  node_pointer q = head->llink;
  while (q->tag & 1)
    q = q->llink;
}

template <typename T>
node_pointer bst<T>::inorder_successor(node_pointer p) const
{
  if (p->tag & 1)
    return p->rlink;

  node_pointer q = p->rlink;
  while (!(q->tag & 2))
    q = q->llink;

  return q;
}

template <typename T>
node_pointer bst<T>::inorder_predecessor(node_pointer p) const
{
  if (p->tag & 2)
    return p->llink;

  node_pointer q = p->llink;
  while (!(q->tag & 1))
    q = q->rlink;

  return q;
}

template <typename T>
bst<T>::bst(std::size_t reserve_n)
: pool(reserve_n == 0 ? 1 : reserve_n)
, avail(0)
{
  head->llink = head;
  head->rlink = head;
  head->tag = 0;
  head->tag = head->tag | 2;

  const size_type pool_size = pool.size();
  // Let us link the avail stack.
  pool[0].llink = 0;
  pool[0].rlink = 0;
  for (std::size_t i = 1; i < pool_size; ++i) {
    pool[i].llink = &pool[i - 1];
    pool[i].rlink = 0;
  }
  avail = &pool.back();
}

template <typename T>
typename bst<T>::node_pointer bst<T>::insert_node_right(T key)
{
  if (!avail)
    return 0; // The tree has exhausted its capacity.

  node_pointer q = avail;
  avail = avail->llink;
  q->key = key;
  q->rlink = p->rlink;
  q->tag = (q->tag & 2) | p->tag & 1;
  p->rlink = q;
  p->tag = p->tag & 2;
  q->llink = p;
  q->tag = q->tag | 2;

  if (!(q->tag & 1)) {
    node_pointer qs = inorder_successor(q);
    qs->llink = q;
  }

  return q;
}

template <typename T>
typename bst<T>::node_pointer bst<T>::insert_node_left(T key)
{
  if (!avail)
    return 0; // The tree has exhausted its capacity.

  node_pointer q = avail;
  avail = avail->llink;
  q->key = key;
  q->llink = p->llink;
  q->tag = (q->tag & 1) | p->tag & 2;
  p->llink = q;
  p->tag = p->tag & 1;
  q->rlink = p;
  q->tag = q->tag | 1;

  if (!(q->tag & 2)) {
    node_pointer qs = inorder_predecessor(q);
    qs->rlink = q;
  }

  return q;
}

template <typename T>
typename bst<T>::node_pointer bst<T>::insert(T key)
{
  if (!head->tag) { // the tree is empty
    head->llink = insert_node_left(key);
    return head->llink;
  }

  node_pointer p = head->llink;
  for (;;) {
    if (key < p->key) {
      if (p->llink) {
        p = p->llink;
        continue;
      }
      p->llink = insert_node_left(key);
      return p->llink;
    } else if (key > p->key) {
      if (p->rlink) {
        p = p->rlink;
        continue;
      }
      p->rlink = insert_node_right(key);
      return p->rlink;
    } else {
      return p;
    }
  }
}

