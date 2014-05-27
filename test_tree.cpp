#include <iostream>

#include "tree.hpp"

int main()
{
  bst<int, 10> t;
  typedef bst<int, 10>::node_pointer node_pointer; 
  node_pointer iter = t.insert(5);
  iter = t.insert(4);
  iter = t.insert(6);
  iter = t.insert(3);
  iter = t.insert(7);
  iter = t.insert(2);
  iter = t.insert(8);
  iter = t.insert(1);
  iter = t.insert(9);
  iter = t.insert(10);
  iter = t.insert(10);
  return 0;
}

