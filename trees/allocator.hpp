#pragma once

#include <exception>

#include "bst_node.hpp"

namespace rtcpp {

template <typename T>
class allocator {
  public:
  typedef T* pointer;
  typedef T* const_pointer;
  typedef bst_node<T> node_type;
  typedef node_type* node_pointer;
  typedef node_type value_type;
  node_pointer allocate() const noexcept
  {
    try {
      return new node_type;
    } catch (const std::bad_alloc& e) {
      return 0;
    }
  }
  void deallocate(node_pointer p) const noexcept {delete p;};
};

}

