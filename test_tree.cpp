#include <iostream>
#include <iterator>
#include <vector>
#include <stack>
#include <random>

#include "tree.hpp"

int main()
{
  const int size = 10;
  const int a = 1;
  const int b = 2000;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(a, b);

  bst<int> t(size);
  typedef bst<int>::node_pointer node_pointer; 
  for (int i = 0; i < size; ++i) {
    node_pointer iter = t.insert(dis(gen));
    if (!iter)
      break;
  }
  node_pointer p = t.get_root();
  if (!p) {
    std::cout << "Error." << std::endl;
    return 1;
  }

  std::vector<int> arr;
  std::stack<node_pointer> s;
  for (;;) {
    while (p) {
      s.push(p);
      p = p->llink;
    }
    if (s.size() == 0)
      break;
    p = s.top();
    s.pop();
    arr.push_back(p->key);
    p = p->rlink;
  }

  typedef bst<int>::iterator iterator;
  std::copy(std::begin(arr), std::end(arr), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
  std::copy(std::begin(t), std::end(t), std::ostream_iterator<int>(std::cout, " "));
  //std::copy(t.begin(), t.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
    
  return 0;
}

