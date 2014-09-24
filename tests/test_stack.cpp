#include <utility>
#include <iostream>

#include "stack.hpp"

int main()
{
  const int size = 10;
  stack<int, size> s;

  for (int i = 0; i < size; ++i)
    s.push(i);

  stack<int, size> ss(s);

  std::cout << ss.size() << std::endl;
  std::cout << std::endl;
  for (int i = 0; i < size; ++i) {
    if (ss.size() != 0) {
      std::cout << ss.top() << std::endl;
      ss.pop();
    }
  }
  std::cout << std::endl;
  std::cout << ss.size() << std::endl;
}

