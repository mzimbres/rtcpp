#pragma once

#include "bst_node.hpp"

template <typename T>
class inorder_iterator {
  public:
  typedef T value_type;
  typedef const T* pointer;
  typedef const T& reference;
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef std::ptrdiff_t difference_type;
  typedef node<T> node_type;
  typedef const node_type* node_pointer;
  private:
  node_pointer m_p;
  public:
  inorder_iterator() : m_p(0) {}
  inorder_iterator(node_pointer root) : m_p(root) {}

  inorder_iterator& operator++()
  {
    node_pointer q = m_p->rlink;
    if (m_p->tag & 1) {
      m_p = q;
      return *this;
    }

    while (!(q->tag & 2))
      q = q->llink;

    m_p = q;
    return *this;
  }

  inorder_iterator operator++(int)
  {
    inorder_iterator tmp(*this);
    operator++();
    return tmp;
  }

  inorder_iterator& operator--()
  {
    node_pointer q = m_p->llink;
    if (m_p->tag & 2) {
      m_p = q;
      return *this;
    }

    while (!(q->tag & 1))
      q = q->rlink;

    m_p = q;
    return *this;
  }

  inorder_iterator operator--(int)
  {
    inorder_iterator tmp(*this);
    operator--();
    return tmp;
  }

  T operator*() const {return m_p->key;}
  bool operator==(const inorder_iterator& rhs) const { return m_p == rhs.m_p; }
  bool operator!=(const inorder_iterator& rhs) const { return !(*this == rhs); }
};

