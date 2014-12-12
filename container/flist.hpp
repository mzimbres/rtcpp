#pragma once

#include <vector>
#include <iterator>
#include <memory>

#include "flist_node.hpp"
#include "flist_iterator.hpp"

namespace rt {

template <typename T, typename Allocator = std::allocator<T>>
class flist {
  public:
  typedef flist_node<T> node_type;
  typedef Allocator allocator_type;
  typedef node_type* node_pointer;
  typedef flist_iterator<T> iterator;
  typedef T value_type;
  private:
  typedef typename std::allocator_traits<Allocator>::template rebind_alloc<node_type> inner_allocator_type;
  inner_allocator_type m_inner_alloc;
  node_pointer head;
  node_pointer tail;
  public:
  iterator begin() {return iterator(head);}
  iterator end() {return iterator(0);}
  flist(const std::allocator<T>& alloc = std::allocator<T>())
  : m_inner_alloc(alloc)
  , head(0)
  , tail(0)
  { }
  node_pointer add_node(T data)
  {
    node_pointer q = std::allocator_traits<inner_allocator_type>::allocate(m_inner_alloc, 1);
    std::allocator_traits<inner_allocator_type>::construct(m_inner_alloc, std::addressof(q->info), data);
    q->info = data;
    return q;
  }
  bool push_back(const T& data)
  {
    if (!head) {
      head = add_node(data);
      tail = head;
      return true;
    }
    const node_pointer q = add_node(data);
    if (!q)
      return false;
    tail->llink = q;
    tail = q;
    return true;
  }
  void remove_if(T value)
  {
    node_pointer* p1 = &head;
    node_pointer p2 = head->llink;
    while (p2) {
      if (p2->info == value) {
        node_pointer tmp = p2->llink;
        std::allocator_traits<inner_allocator_type>::destroy(m_inner_alloc, &p2->info);
        std::allocator_traits<inner_allocator_type>::deallocate(m_inner_alloc, p2, 1);
        p2 = tmp;
        *p1 = p2;
        continue;
      }
      tail = p2;
      p1 = &p2->llink;
      p2 = p2->llink;
    }
  }
};

}

