#pragma once

#include <stack>

template <typename T, template <typename> class Node>
class inorder_iterator : public std::iterator<std::forward_iterator_tag, T> {
  public:
  typedef typename std::iterator<std::forward_iterator_tag, T>::pointer pointer;
  typedef typename std::iterator<std::forward_iterator_tag, T>::value_type value_type;
  typedef Node<T> node_type;
  typedef node_type* node_pointer;
  private:
  std::stack<node_pointer> s;
  node_pointer p;
  node_pointer tmp;
  public:
  inorder_iterator()
  : p(0)
  , tmp(0)
  {}
  inorder_iterator(node_pointer root)
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
    if (s.size() == 0) {
      p = 0;
      tmp = 0;
      return *this;
    }

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
  bool operator==(const inorder_iterator& rhs) const
  {
    return p == rhs.p && tmp == rhs.tmp && s.size() == rhs.s.size();
  }
  bool operator!=(const inorder_iterator& rhs) const { return !(*this == rhs); }
};

