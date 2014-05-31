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

template <typename T>
class inorder_iterator : public std::iterator<std::forward_iterator_tag, T> {
  public:
  typedef typename std::iterator<std::forward_iterator_tag, T>::pointer pointer;
  typedef typename std::iterator<std::forward_iterator_tag, T>::value_type value_type;
  typedef node<T> node_type;
  typedef node_type* node_pointer;
  private:
  std::stack<node_pointer> s;
  node_pointer p;
  node_pointer tmp;
  bool eq;

  public:
  //inorder_iterator(const inorder_iterator<T>& rhs)
  //{
  //  p = rhs.p;
  //}
  //const inorder_iterator<T>& operator=(const inorder_iterator<T>& rhs)
  //{
  //  if (this != &rhs)
  //    p = rhs.p;

  //  return *this;
  //}
  inorder_iterator()
  : p(0)
  , tmp(0)
  , eq(false)
  {}
  inorder_iterator(node_pointer root)
  : eq(true)
  { 
    p = root;
    while (p) {
      s.push(p);
      p = p->llink;
    }
    p = s.top();
    s.pop();
    tmp = p;
    p = p->rlink;
  }

  inorder_iterator& operator++()
  {
    while (p) {
      s.push(p);
      p = p->llink;
    }
    if (s.size() == 0)
      return *this;

    p = s.top();
    tmp = p;
    s.pop();
    tmp = p;
    p = p->rlink;

    return *this;
  }
  inorder_iterator operator++(int)
  {
    inorder_iterator tmp(*this);
    operator++();
    return tmp;
  }
  T operator*() const {return tmp->key;}
  bool operator==(const inorder_iterator<T>& rhs) const
  {
    return p == rhs.p && tmp == rhs.tmp && s.size() == rhs.s.size() && eq && rhs.eq;
  }
  bool operator!=(const inorder_iterator<T>& rhs) const { return !(*this == rhs); }
};

template <typename T>
class bst {
  public:
  typedef std::size_t size_type;
  typedef node<T> node_type;
  typedef node_type* node_pointer;
  typedef inorder_iterator<T> iterator;
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
  iterator begin() const {return inorder_iterator<T>(root);}
  iterator end() const {return inorder_iterator<T>();}
};

