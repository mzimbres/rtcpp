#pragma once

namespace rtcpp {

template <typename Pool>
class pool_allocator {
  private:
  Pool* pool;
  public:
  typedef typename Pool::pointer pointer;
  typedef typename Pool::const_pointer const_pointer;
  typedef typename Pool::node_type node_type;
  typedef typename Pool::node_pointer node_pointer;
  pool_allocator(Pool* p) noexcept : pool(p) {}
  pool_allocator() noexcept : pool(0) {}
  node_pointer allocate() noexcept {return pool->pop();}
  void deallocate(node_pointer p) noexcept {pool->push(p);};
};

}

