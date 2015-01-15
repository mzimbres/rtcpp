#pragma once

#include <iterator>

#include "bst_node.hpp"

namespace rt {

template <typename T>
class bst_iterator : public std::iterator<std::bidirectional_iterator_tag, const T> {
  public:
  typedef bst_node<T> node_type;
  typedef const node_type* node_pointer;
  private:
  node_pointer m_p;
  public:
  bst_iterator() noexcept : m_p(0) {}
  bst_iterator(node_pointer root) noexcept : m_p(root) {}

  bst_iterator& operator++() noexcept
  {
    m_p = inorder<1>(m_p);
    return *this;
  }

  bst_iterator operator++(int) noexcept
  {
    bst_iterator tmp(*this);
    operator++();
    return tmp;
  }

  bst_iterator& operator--() noexcept
  {
    m_p = inorder<0>(m_p);
    return *this;
  }

  bst_iterator operator--(int) noexcept
  {
    bst_iterator tmp(*this);
    operator--();
    return tmp;
  }

  T operator*() const noexcept {return m_p->key;}
  bool operator==(const bst_iterator& rhs) const noexcept { return m_p == rhs.m_p; }
  bool operator!=(const bst_iterator& rhs) const noexcept { return !(*this == rhs); }
};

}

