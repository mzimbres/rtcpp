#pragma once

namespace rtcpp {

template <typename Pool>
class pool_allocator {
  Pool* pool;
  public:
  typedef typename Pool::node_type node_type;
  typedef typename Pool::pointer pointer;
  pool_allocator(Pool* p) : pool(p) {}
  pool_allocator() : pool(0) {}
  node_type* allocate() {return pool->allocate();}
  void deallocate(pointer p) {pool->deallocate(p);};
};

}

