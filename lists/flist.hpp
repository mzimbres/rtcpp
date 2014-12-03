#pragma once

#include <vector>
#include <iterator>

#include "flist_node.hpp"
#include "flist_iterator.hpp"

namespace rtcpp {

template <typename T>
class flist {
  public:
  typedef flist_node<T> node_type;
  typedef node_type* node_pointer;
  typedef flist_iterator<T> iterator;
  typedef T value_type;
  private:
  std::vector<flist_node<T>> pool;
  node_pointer avail;
  node_pointer head;
  node_pointer tail;
  public:
  iterator begin() {return iterator(head);}
  iterator end() {return iterator(0);}
  flist(std::size_t n)
  : pool(n)
  , avail(0)
  , head(0)
  , tail(0)
  {
    pool[0].llink = 0;
    for (std::size_t i = 0; i < n; ++i)
      pool[i].llink = &pool[i - 1];
    avail = &pool.back();
  }
  node_pointer add_node(T data)
  {
    if (!avail)
      return 0;

    node_pointer q = avail;
    avail = avail->llink;
    q->info = data;
    q->llink = 0;
    return q;
  }
  bool push_back(T data)
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
        p2->llink = avail;
        avail = p2;
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

