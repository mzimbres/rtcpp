#pragma once

#include "bst_node.hpp"

namespace rtcpp {

template <typename Node>
class node_stack {
  public:
  typedef typename Node::value_type* pointer;
  typedef const typename Node::value_type* const_pointer;
  typedef Node node_type;
  typedef Node* node_pointer;
  private:
  typedef std::vector<node_type> pool_type;
  std::vector<node_type> pool;
  node_pointer avail;
  public:
  typedef typename pool_type::size_type size_type;
  node_stack(size_type n);
  node_pointer pop() noexcept;
  void push(node_pointer p) noexcept;
  size_type capacity() const noexcept {return pool.capacity();}
};

template <typename T>
node_stack<T>::node_stack(size_type n)
: pool(n == 0 ? 10 : n)
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
typename node_stack<T>::node_pointer node_stack<T>::pop() noexcept
{
  node_pointer q = avail;
  if (avail)
    avail = avail->llink;
  return q;
}

template <typename T>
void node_stack<T>::push(typename node_stack<T>::node_pointer p) noexcept
{
  if (!p)
    return;

  p->rlink = 0;
  p->llink = avail;
  avail = p;
}

}

