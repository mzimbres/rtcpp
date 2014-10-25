#pragma once

#include <iterator>

#include "bst_node.hpp"
#include "link_stack.hpp"

namespace rtcpp {

template <typename Node>
class node_stack {
  public:
  typedef typename Node::value_type* pointer;
  typedef const typename Node::value_type* const_pointer;
  typedef Node node_type;
  typedef Node* node_pointer;
  typedef Node value_type;
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
, avail(link_stack(std::begin(pool), std::end(pool)))
{}

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

