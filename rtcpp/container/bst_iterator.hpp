#pragma once

#include <iterator>

#include "bst_node.hpp"

namespace rt {

template <typename T, typename Ptr>
class bst_iterator :
  public std::iterator<std::bidirectional_iterator_tag, const T> {
  public:
  Ptr m_p;
  bst_iterator() noexcept : m_p(0) {}
  bst_iterator(Ptr root) noexcept : m_p(root) {}

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
};

template <typename T, typename Ptr>
bool operator==( const bst_iterator<T, Ptr>& rhs
               , const bst_iterator<T, Ptr>& lhs) noexcept
{ return lhs.m_p == rhs.m_p; }

template <typename T, typename Ptr>
bool operator!=( const bst_iterator<T, Ptr>& rhs
               , const bst_iterator<T, Ptr>& lhs) noexcept
{ return !(lhs == rhs); }

}

