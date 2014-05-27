#include <utility>
#include <iostream>

#include "stack.hpp"

int main()
{
  const int size = 10;
  stack<int, size> s;

  for (int i = 0; i < size; ++i)
    s.push(i);

  s.pop();
  s.pop();
  s.pop();
  s.pop();

  s.push(20);
  s.push(30);
  s.push(40);
  s.push(50);

  std::cout << s.in_use() << std::endl;
  std::cout << std::endl;
  for (int i = 0; i < size; ++i) {
    if (s.in_use() != 0) {
      std::cout << s.top() << std::endl;
      s.pop();
    }
  }
  std::cout << std::endl;
  std::cout << s.in_use() << std::endl;
}

