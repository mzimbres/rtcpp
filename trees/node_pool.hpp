#pragma once

#include "bst_node.hpp"

namespace rtcpp {

template <typename T>
class node_pool {
  private:
  typedef node<T> node_type;
  typedef node_type* pointer;
  typedef std::vector<node_type> pool_type;
  std::vector<node_type> pool;
  pointer avail;
  std::size_t counter;
  public:
  typedef typename pool_type::size_type size_type;
  node_pool(size_type n);
  node_type* allocate();
  void deallocate(pointer p);
  size_type size() const {return pool.size();}
  size_type available() const {return counter;}
};

template <typename T>
node_pool<T>::node_pool(size_type n)
: pool(n == 0 ? 10 : n)
, counter(n)
{
  const size_type pool_size = pool.size();
  // Let us link the avail stack.
  pool[0].llink = 0;
  pool[0].rlink = 0;
  for (std::size_t i = 1; i < pool_size; ++i) {
    pool[i].llink = &pool[i - 1];
    pool[i].rlink = 0;
  }
  avail = &pool.back();
}

template <typename T>
typename node_pool<T>::pointer node_pool<T>::allocate()
{
  pointer q = avail;
  if (avail)
    avail = avail->llink;
  --counter;
  return q;
}

template <typename T>
void node_pool<T>::deallocate(typename node_pool<T>::pointer p)
{
  if (!p)
    return;

  p->rlink = 0;
  p->llink = avail;
  avail = p;
  ++counter;
}

}

