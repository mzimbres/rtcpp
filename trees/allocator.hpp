#pragma once

#include "bst_node.hpp"

namespace rtcpp {

template <typename T>
class allocator {
  public:
  typedef T* pointer;
  typedef T* const_pointer;
  typedef node<T> node_type;
  typedef node_type* node_pointer;
  node_pointer allocate() {return new node_type;}
  void deallocate(node_pointer p) {delete p;};
};

}

