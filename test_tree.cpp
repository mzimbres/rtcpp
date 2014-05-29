#include <iostream>
#include <iterator>
#include <array>

#include "tree.hpp"

int main()
{
  bst<int, 10> t;
  typedef bst<int, 10>::node_pointer node_pointer; 
  for (int i = 0; i < 20; ++i) {
    node_pointer iter = t.insert(i);
    if (!iter)
      break;
  }
  node_pointer p = t.get_root();
  if (!p) {
    return 1;
  }

  std::array<int, 10> arr;
  stack<node_pointer, 10> s;
  int i = 0;
  for (;;) {
    if (p) {
      s.push(p);
      p = p->llink;
      continue;
    }
    if (s.size() == 0)
      break;
    p = s.top();
    s.pop();
    arr[i++] = p->key;
    p = p->rlink;
  }

  std::copy(std::begin(arr), std::end(arr), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
    
  return 0;
}

