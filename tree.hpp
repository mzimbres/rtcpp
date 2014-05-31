#include <iostream>
#include <iterator>
#include <vector>

#include "stack.hpp"

template <typename T>
struct node {
  typedef T key_type;
  key_type key;
  node* llink;
  node* rlink;
};

//template <typename T, std::size_t N>
//class inorder_iterator : public std::iterator<std::forward_iterator_tag, T> {
//  public:
//  typedef typename std::iterator<std::forward_iterator_tag, T>::pointer pointer;
//  typedef typename std::iterator<std::forward_iterator_tag, T>::value_type value_type;
//  typedef typename T::key_type key_type;
//  private:
//  stack<pointer, N> s;
//  pointer p;
//  public:
//  inorder_iterator(pointer node)
//  { 
//    p = node;
//    while (p) {
//      s.push(p);
//      p = p->llink;
//    }
//    p = s.top();
//    s.pop();
//  };
//  inorder_iterator& operator++()
//  {
//    if (p) {
//      s.push(p);
//      p = p->llink;
//      return *this;
//    }
//
//    if (s.size() == 0)
//      return *this;
//
//    p = s.top();
//    s.pop();
//    p = p->rlink;
//    return *this;
//  }
//  inorder_iterator operator++(int)
//  {
//    inorder_iterator tmp(*this);
//    operator++();
//    return tmp;
//  }
//  key_type operator*() const {return p->key;}
//};

template <typename T>
class bst {
  public:
  typedef std::size_t size_type;
  typedef node<T> node_type;
  typedef node_type* node_pointer;
  private:
  std::vector<node_type> pool;
  node_pointer root;
  node_pointer avail;
  node_pointer add_node(T key)
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
  public:
  node_pointer get_root() const {return root;}
  bst(std::size_t size)
  : pool(size)
  , root(0)
  , avail(0)
  {
    // Let us link the avail stack.
    pool[0].llink = 0;
    pool[0].rlink = 0;
    for (size_type i = 1; i < pool.size(); ++i) {
      pool[i].llink = &pool[i - 1];
      pool[i].rlink = 0;
    }
    avail = &pool.back();
  }
  node_pointer insert(T key)
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
};

