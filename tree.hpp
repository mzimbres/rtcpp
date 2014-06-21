#include <iostream>
#include <vector>
#include <stack>

#include "inorder_iterator.hpp"

template <typename T>
struct node {
  T key;
  node* llink;
  node* rlink;
};

template <typename T>
class bst {
  public:
  typedef node<T> node_type;
  typedef node_type* node_pointer;
  typedef inorder_iterator<T, node > const_iterator;
  private:
  std::vector<node_type> pool;
  node_pointer root;
  node_pointer avail;
  node_pointer add_node(T key);
  bst(const bst& rhs); // To be implemented
  const bst& operator=(const bst& rhs); // To be implemented
  public:
  node_pointer get_root() const {return root;}
  bst(std::size_t size);
  node_pointer insert(T key);
  const_iterator begin() const {return const_iterator(root);}
  const_iterator end() const {return const_iterator();}
};

template <typename T>
bst<T>::bst(std::size_t size)
: pool(size)
, root(0)
, avail(0)
{
  // Let us link the avail stack.
  pool[0].llink = 0;
  pool[0].rlink = 0;
  for (std::size_t i = 1; i < pool.size(); ++i) {
    pool[i].llink = &pool[i - 1];
    pool[i].rlink = 0;
  }
  avail = &pool.back();
}

template <typename T>
typename bst<T>::node_pointer bst<T>::add_node(T key)
{
  if (!avail)
    return 0; // The tree has exhausted its capacity.

  node_pointer q = avail;
  avail = avail->llink;
  q->key = key;
  q->llink = 0;
  q->rlink = 0;
  return q;
}

template <typename T>
typename bst<T>::node_pointer bst<T>::insert(T key)
{
  if (!root) { // the tree is empty
    root = add_node(key);
    return root;
  }
  // We have enough space lets insert the element.
  node_pointer p = root;
  for (;;) {
    if (key < p->key) {
      if (p->llink) {
        p = p->llink;
        continue;
      }
      p->llink = add_node(key);
      return p->llink;
    } else if (key > p->key) {
      if (p->rlink) {
        p = p->rlink;
        continue;
      }
      p->rlink = add_node(key);
      return p->rlink;
    } else {
      return p;
    }
  }
}

