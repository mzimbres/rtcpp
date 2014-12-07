#pragma once

#include <vector>
#include <iterator>

#include "flist_node.hpp"

namespace rtcpp {

template <typename T>
class flist_iterator : public std::iterator<std::forward_iterator_tag, T> {
  public:
  typedef flist_node<T> node_type;
  typedef node_type* node_pointer;
  private:
  node_pointer head;
  public:
  flist_iterator(node_pointer h)
  : head(h)
  {}
  flist_iterator& operator++()
  {
    head = head->llink;
    return *this;
  }
  flist_iterator operator++(int)
  {
    flist_iterator tmp(*this);
    operator++();
    return tmp;
  }
  T operator*() const { return head->info; }
  bool operator==(const flist_iterator<T>& rhs) {return head == rhs.head;}
  bool operator!=(const flist_iterator<T>& rhs) {return !(*this == rhs);}
};

}

