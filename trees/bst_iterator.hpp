#pragma once

#include "bst_node.hpp"

namespace rtcpp {

template <typename T>
class bst_iterator {
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
  bst_iterator() : m_p(0) {}
  bst_iterator(node_pointer root) : m_p(root) {}

  bst_iterator& operator++()
  {
    m_p = inorder_successor(m_p);
    return *this;
  }

  bst_iterator operator++(int)
  {
    bst_iterator tmp(*this);
    operator++();
    return tmp;
  }

  bst_iterator& operator--()
  {
    m_p = inorder_predecessor(m_p);
    return *this;
  }

  bst_iterator operator--(int)
  {
    bst_iterator tmp(*this);
    operator--();
    return tmp;
  }

  T operator*() const {return m_p->key;}
  bool operator==(const bst_iterator& rhs) const { return m_p == rhs.m_p; }
  bool operator!=(const bst_iterator& rhs) const { return !(*this == rhs); }
};

}

