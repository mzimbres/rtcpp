#pragma once

#include <exception>

#include "bst_node.hpp"

namespace rtcpp {

template <typename T>
class allocator {
  public:
  typedef T value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  pointer allocate() const noexcept
  {
    try {
      return new value_type;
    } catch (const std::bad_alloc& e) {
      return 0;
    }
  }
  void deallocate(pointer p) const noexcept {delete p;};

  template <typename U>
  struct rebind {
    typedef allocator<U> other;
  };
};

}

