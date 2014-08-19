#include <iostream>
#include <iterator>
#include <vector>
#include <stack>
#include <random>
#include <limits>
#include <functional>

#include "bst.hpp"
#include "to_number.hpp"

//template <typename OutputIterator>
//void inorder_traversal(OutputIterator begin, std::size_t n)
//{
//  std::stack<node_pointer> s;
//  for (;;) {
//    while (p) {
//      s.push(p);
//      p = p->llink;
//    }
//    if (s.size() == 0)
//      break;
//    p = s.top();
//    s.pop();
//    arr.push_back(p->key);
//    p = p->rlink;
//  }
//}

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: ./a.out N" << std::endl;
    return 1;
  }
  
  const int size = to_number<int>(argv[1]);
  const int a = 1;
  //const int b = std::numeric_limits<int>::max();
  const int b = 10;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(a, b);

  bst<int> t(size);
  auto pair = t.insert(4);
  pair = t.insert(1);
  pair = t.insert(3);
  pair = t.insert(10);
  pair = t.insert(7);

  auto begin = std::begin(t);
  auto end = std::end(t);
  std::copy(begin, end, std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
    
  return 0;
}

