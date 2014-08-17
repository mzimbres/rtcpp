#pragma once

#include "bst_node.hpp"

template <typename T>
class inorder_iterator : public std::iterator<std::forward_iterator_tag, T> {
  public:
  typedef typename std::iterator<std::forward_iterator_tag, T>::pointer pointer;
  typedef typename std::iterator<std::forward_iterator_tag, T>::value_type value_type;
  typedef node<T> node_type;
  typedef const node_type* node_pointer;
  private:
  node_pointer p;
  node_pointer q;
  public:
  inorder_iterator()
  : p(0)
  , q()
  {}
  inorder_iterator(node_pointer root)
  : p(root)
  , q(0)
  {}

  inorder_iterator& operator++()
  {
    p = p->rlink;

    while (!(p->tag & 2))
      p = p->llink;

    q = p;
    return *this;
  }
  inorder_iterator operator++(int)
  {
    inorder_iterator tmp(*this);
    operator++();
    return tmp;
  }
  T operator*() const {return q->key;}
  bool operator==(const inorder_iterator& rhs) const { return p == rhs.p; }
  bool operator!=(const inorder_iterator& rhs) const { return !(*this == rhs); }
};

