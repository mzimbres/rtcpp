#pragma once

#include "bst_node.hpp"

namespace rtcpp {

template <typename T>
class node_pool {
  public:
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef node<T> node_type;
  typedef node_type* node_pointer;
  private:
  typedef std::vector<node_type> pool_type;
  std::vector<node_type> pool;
  node_pointer avail;
  std::size_t counter;
  public:
  typedef typename pool_type::size_type size_type;
  node_pool(size_type n);
  node_pointer allocate();
  void deallocate(node_pointer p);
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
typename node_pool<T>::node_pointer node_pool<T>::allocate()
{
  node_pointer q = avail;
  if (avail)
    avail = avail->llink;
  --counter;
  return q;
}

template <typename T>
void node_pool<T>::deallocate(typename node_pool<T>::node_pointer p)
{
  if (!p)
    return;

  p->rlink = 0;
  p->llink = avail;
  avail = p;
  ++counter;
}

}

