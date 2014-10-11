#pragma once

#include "bst_node.hpp"

namespace rtcpp {

template <typename T>
class allocator {
  private:
  typedef node<T> node_type;
  typedef node_type* pointer;
  public:
  node_type* allocate() {return new node_type;}
  void deallocate(pointer p) {delete p;};
};

}

